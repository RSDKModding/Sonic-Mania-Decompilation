#include "RetroEngine.hpp"

#if RETRO_PLATFORM == RETRO_WIN
#include <Windows.h>

#undef PRINT_ERROR //causes conflicts
#endif
#if RETRO_PLATFORM == RETRO_ANDROID
#include <android/log.h>
#endif

bool32 engineDebugMode = true;
bool32 useEndLine = true;
char outputString[0x400];

#if RETRO_REV02
int32 debugValCnt = 0;
DebugValueInfo debugValues[DEBUGVAL_MAX];
#endif

DevMenu devMenu = DevMenu();

void PrintLog(SeverityModes severity, const char *message, ...)
{
#ifndef RETRO_DISABLE_LOG
    if (engineDebugMode) {
        // make the full string
        va_list args;
        va_start(args, message);
        vsprintf(outputString, message, args);
        if (useEndLine)
            sprintf(outputString, "%s\n", outputString);
        else
            sprintf(outputString, "%s", outputString);
        va_end(args);

#if RETRO_REV02
        switch (severity) {
            case PRINT_NORMAL:
            default: break;
                break;
            case PRINT_POPUP:
                if (sceneInfo.state & 3) {
                    CreateEntity(RSDK::DevOutput->objectID, outputString, 0, 0);
                }
                break;
            case PRINT_ERROR:
                if (sceneInfo.state & 3) {
                    engine.prevEngineMode = sceneInfo.state;
                    sceneInfo.state       = ENGINESTATE_ERRORMSG;
                }
                break;
            case PRINT_FATAL:
                if (sceneInfo.state & 3) {
                    engine.prevEngineMode = sceneInfo.state;
                    sceneInfo.state       = ENGINESTATE_ERRORMSG_FATAL;
                }
                break;
        }
#endif
        if (engine.printConsole) {
            printConsole(outputString);
        }
        else {
#if RETRO_PLATFORM == RETRO_WIN
            OutputDebugStringA(outputString);
#elif RETRO_PLATFORM == RETRO_ANDROID
            int as = ANDROID_LOG_INFO;
            switch (severity) {
                case PRINT_ERROR: as = ANDROID_LOG_ERROR; break;
                case PRINT_FATAL: as = ANDROID_LOG_FATAL; break;
                default: break;
            }
            __android_log_print(as, "RSDKv5", "%s", outputString);
#elif RETRO_PLATFORM == RETRO_SWITCH
            printf("%s", outputString);
#endif
        }

#if !RETRO_USE_ORIGINAL_CODE
        char pathBuffer[0x100];
        sprintf(pathBuffer, BASE_PATH "log.txt");
        FileIO *file = fOpen(pathBuffer, "a");
        if (file) {
            fWrite(&outputString, 1, strlen(outputString), file);
            fClose(file);
        }
#endif

    }
#endif
}

#if RETRO_REV02
void SetDebugValue(const char *name, void *valPtr, int32 type, int32 min, int32 max)
{
    if (debugValCnt < DEBUGVAL_MAX) {
        DebugValueInfo *value = &debugValues[debugValCnt++];
        strncpy(value->name, name, 0x10);
        value->value = valPtr;
        switch (type) {
            case DTYPE_BOOL: // bool
                value->type       = 0;
                value->valByteCnt = 4;
                break;
            case DTYPE_UINT8: // byte
                value->type       = 1;
                value->valByteCnt = 1;
                break;
            case DTYPE_UINT16: // ushort
                value->type       = 1;
                value->valByteCnt = 2;
                break;
            case DTYPE_UINT32: // uint
                value->type       = 1;
                value->valByteCnt = 4;
                break;
            case DTYPE_INT8: // sbyte
                value->type       = 2;
                value->valByteCnt = 1;
                break;
            case DTYPE_INT16: // short
                value->type       = 2;
                value->valByteCnt = 2;
                break;
            case DTYPE_INT32: // int
                value->type       = 2;
                value->valByteCnt = 4;
                break;
            default: break;
        }
        value->min = min;
        value->max = max;
    }
}
#endif

#if !RETRO_REV02
void PrintMessage(void *msg, int type)
{
    if (msg && engine.printConsole) {
        switch (type) {
            case 0: PrintLog(PRINT_NORMAL, "%s", (const char *)msg); break;
            case 1: PrintLog(PRINT_NORMAL, "%i", *(signed int *)msg); break;
            case 2: PrintLog(PRINT_NORMAL, "%i", *(int *)msg, 0); break;
            case 3: PrintLog(PRINT_NORMAL, "%f", *(float *)msg); break;
            default: break;
        }
    }
}
#endif

#if !RETRO_USE_ORIGINAL_CODE
byte touchTimer = 0;

void DevMenu_HandleTouchControls() {
    if (!controller[CONT_P1].keyStart.down && !controller[CONT_P1].keyUp.down && !controller[CONT_P1].keyDown.down) {
        for (int t = 0; t < touchMouseData.count; ++t) {
            if (touchMouseData.down[t] && !(touchTimer % 8)) {
                int tx = touchMouseData.x[t] * screens->size.x;
                int ty = touchMouseData.y[t] * screens->size.y;

                if (tx < screens->center.x) {
                    if (ty >= screens->center.y) {
                        if (!controller[CONT_P1].keyDown.down)
                            controller[CONT_P1].keyDown.press = true;
                        controller[CONT_P1].keyDown.down = true;
                        break;
                    }
                    else {
                        if (!controller[CONT_P1].keyUp.down)
                            controller[CONT_P1].keyUp.press = true;
                        controller[CONT_P1].keyUp.down = true;
                        break;
                    }
                }
                else if (tx > screens->center.x) {
                    if (ty > screens->center.y) {
                        if (!controller[CONT_P1].keyStart.down)
                            controller[CONT_P1].keyStart.press = true;
                        controller[CONT_P1].keyStart.down = true;
                        break;
                    }
                    else {
                        if (!controller[CONT_P1].keyB.down)
                            controller[CONT_P1].keyB.press = true;
                        controller[CONT_P1].keyB.down = true;
                        break;
                    }
                }
            }
        }
    }

    touchTimer++;
}
#endif

void DevMenu_MainMenu()
{
#if !RETRO_USE_MOD_LOADER
    int optionCount               = 5;
    uint optionColours[]          = { 0x808090, 0x808090, 0x808090, 0x808090, 0x808090 };
    const char *optionNames[]     = { "Resume", "Restart", "Stage Select", "Options", "Exit" };
#else
    int optionCount           = 6;
    uint optionColours[]      = { 0x808090, 0x808090, 0x808090, 0x808090, 0x808090, 0x808090 };
    const char *optionNames[] = { "Resume", "Restart", "Stage Select", "Options", "Mods", "Exit" };
#endif
    optionColours[devMenu.option] = 0xF0F0F0;

    // Info Box
    int y = currentScreen->center.y - 80;
    DrawRectangle(currentScreen->center.x - 128, currentScreen->center.y - 84, 256, 48, 128, 255, INK_NONE, true);
    DrawDevText("RETRO ENGINE v5", currentScreen->center.x, y, ALIGN_CENTER, 0xF0F0F0);
    y += 8;
    DrawDevText("Dev Menu", currentScreen->center.x, y, ALIGN_CENTER, 0xF0F0F0);
    y += 16;
    DrawDevText(RSDK::gameVerInfo.gameName, currentScreen->center.x, y, ALIGN_CENTER, 0x808090);
    y += 8;
    DrawDevText(RSDK::gameVerInfo.gameVersion, currentScreen->center.x, y, ALIGN_CENTER, 0x808090);
    y += 24;

    // Options Box
    DrawRectangle(currentScreen->center.x - 128, y - 8, 256, 72, 128, 255, INK_NONE, true);

#if RETRO_USE_MOD_LOADER
    y -= 6;
#endif
    for (int i = 0; i < optionCount; ++i) {
        DrawDevText(optionNames[i], currentScreen->center.x, y, ALIGN_CENTER, optionColours[i]);
        y += 12;
    }
    y += 20;

    // Storage box

    // Stage Storage
    int stgUsed = (sizeof(int) * RSDK::dataStorage[RSDK::DATASET_STG].usedStorage) / (float)RSDK::dataStorage[RSDK::DATASET_STG].storageLimit * 126.0;
    DrawRectangle(currentScreen->center.x - 40, y, 128, 8, 128, 255, INK_NONE, true);
    DrawRectangle(currentScreen->center.x - 39, y + 1, stgUsed, 6, 0xF0F0F0, 255, INK_NONE, true);
    DrawDevText("STG", currentScreen->center.x - 64, y, 0, 0xF0F080);

    // Music Storage
    int musUsed = (sizeof(int) * RSDK::dataStorage[RSDK::DATASET_MUS].usedStorage) / (float)RSDK::dataStorage[RSDK::DATASET_MUS].storageLimit * 126.0;
    y += 10;
    DrawRectangle(currentScreen->center.x - 40, y, 128, 8, 128, 255, INK_NONE, true);
    DrawRectangle(currentScreen->center.x - 39, y + 1, musUsed, 6, 0xF0F0F0, 255, INK_NONE, true);
    DrawDevText("MUS", currentScreen->center.x - 64, y, 0, 0xF0F080);

    // SoundFX Storage
    int sfxUsed = (sizeof(int) * RSDK::dataStorage[RSDK::DATASET_SFX].usedStorage) / (float)RSDK::dataStorage[RSDK::DATASET_SFX].storageLimit * 126.0;
    y += 10;
    DrawRectangle(currentScreen->center.x - 40, y, 128, 8, 128, 255, INK_NONE, true);
    DrawRectangle(currentScreen->center.x - 39, y + 1, sfxUsed, 6, 0xF0F0F0, 255, INK_NONE, true);
    DrawDevText("SFX", currentScreen->center.x - 64, y, 0, 0xF0F080);

    // String Storage
    int strUsed = (sizeof(int) * RSDK::dataStorage[RSDK::DATASET_STR].usedStorage) / (float)RSDK::dataStorage[RSDK::DATASET_STR].storageLimit * 126.0;
    y += 10;
    DrawRectangle(currentScreen->center.x - 40, y, 128, 8, 128, 255, INK_NONE, true);
    DrawRectangle(currentScreen->center.x - 39, y + 1, strUsed, 6, 0xF0F0F0, 255, INK_NONE, true);
    DrawDevText("STR", currentScreen->center.x - 64, y, 0, 0xF0F080);

    // Temp Storage
    int tmpUsed = (sizeof(int) * RSDK::dataStorage[RSDK::DATASET_TMP].usedStorage) / (float)RSDK::dataStorage[RSDK::DATASET_TMP].storageLimit * 126.0;
    y += 10;
    DrawRectangle(currentScreen->center.x - 40, y, 128, 8, 128, 255, INK_NONE, true);
    DrawRectangle(currentScreen->center.x - 39, y + 1, tmpUsed, 6, 0xF0F0F0, 255, INK_NONE, true);
    DrawDevText("TMP", currentScreen->center.x - 64, y, 0, 0xF0F080);

#if !RETRO_USE_ORIGINAL_CODE
    DevMenu_HandleTouchControls();
#endif

    if (controller[CONT_P1].keyUp.press) {
        devMenu.option--;
        devMenu.timer = 1;
        if (devMenu.option < 0)
            devMenu.option += optionCount;
    }
    else if (controller[CONT_P1].keyUp.down) {
        if (devMenu.timer) {
            devMenu.timer = ++devMenu.timer & 7;
        }
        else {
            devMenu.option--;
            devMenu.timer = ++devMenu.timer & 7;
            if (devMenu.option < 0)
                devMenu.option += optionCount;
        }
    }

    if (controller[CONT_P1].keyDown.press) {
        devMenu.option++;
        devMenu.timer = 1;
        if (devMenu.option >= optionCount)
            devMenu.option -= optionCount;
    }
    else if (controller[CONT_P1].keyDown.down) {
        if (devMenu.timer) {
            devMenu.timer = ++devMenu.timer & 7;
        }
        else {
            devMenu.option++;
            devMenu.timer = ++devMenu.timer & 7;
            if (devMenu.option >= optionCount)
                devMenu.option -= optionCount;
        }
    }

    bool32 confirm = controller[CONT_P1].keyA.press;
#if RETRO_REV02
    if (RSDK::SKU::userCore->GetConfirmButtonFlip())
#else
    if (RSDK::SKU::GetConfirmButtonFlip())
#endif
        confirm = controller[CONT_P1].keyB.press;

    if (controller[CONT_P1].keyStart.press || confirm) {
        switch (devMenu.option) {
            case 0:
                sceneInfo.state = devMenu.stateStore;
                break;
            case 1:
                sceneInfo.state = ENGINESTATE_LOAD;
                break;
            case 2:
                devMenu.state  = DevMenu_ListSel;
                devMenu.option = 0;
                devMenu.timer  = 1;
                break;
            case 3:
                devMenu.state  = DevMenu_Options;
                devMenu.option = 0;
                devMenu.timer  = 1;
                break;
#if !RETRO_USE_MOD_LOADER
            case 4: engine.running = false; break;
#else
            case 4:
                RSDK::loadMods(); // reload our mod list real quick
                devMenu.state  = DevMenu_Mods;
                devMenu.option = 0;
                devMenu.timer  = 1;
                break;
            case 5: engine.running = false; break;
#endif
            default: break;
        }
    }
}
void DevMenu_ListSel()
{
    int dy = currentScreen->center.y;
    DrawRectangle(currentScreen->center.x - 128, dy - 84, 256, 48, 128, 255, INK_NONE, true);
    dy -= 68;
    DrawDevText("SELECT STAGE CATEGORY", currentScreen->center.x, dy, ALIGN_CENTER, 0xF0F0F0);
    DrawRectangle(currentScreen->center.x - 128, dy + 36, 256, 72, 128, 255, INK_NONE, true);

    uint optionColours[8];
    optionColours[0]                               = 0x808090;
    optionColours[1]                               = 0x808090;
    optionColours[2]                               = 0x808090;
    optionColours[3]                               = 0x808090;
    optionColours[4]                               = 0x808090;
    optionColours[5]                               = 0x808090;
    optionColours[6]                               = 0x808090;
    optionColours[7]                               = 0x808090;
    optionColours[devMenu.option - devMenu.scroll] = 0xF0F0F0;

    int y = dy + 40;
    for (int i = 0; i < 8; ++i) {
        if (devMenu.scroll + i < sceneInfo.categoryCount) {
            DrawDevText(sceneInfo.listCategory[devMenu.scroll + i].name, currentScreen->center.x - 64, y, 0, optionColours[i]);
            y += 8;
        }
    }

#if !RETRO_USE_ORIGINAL_CODE
    DevMenu_HandleTouchControls();
#endif

    if (controller[CONT_P1].keyUp.press) {
        if (--devMenu.option < 0)
            devMenu.option += sceneInfo.categoryCount;
        if (devMenu.option >= devMenu.scroll) {
            if (devMenu.option > devMenu.scroll + 7) {
                devMenu.scroll = devMenu.option - 7;
            }
        }
        else {
            devMenu.scroll = devMenu.option;
        }
        devMenu.timer = 1;
    }
    else if (controller[CONT_P1].keyUp.down) {
        if (devMenu.timer) {
            devMenu.timer = (devMenu.timer + 1) & 7;
            if (devMenu.option >= devMenu.scroll) {
                if (devMenu.option > devMenu.scroll + 7) {
                    devMenu.scroll = devMenu.option - 7;
                }
            }
            else {
                devMenu.scroll = devMenu.option;
            }
        }
        else {
            if (--devMenu.option < 0)
                devMenu.option += sceneInfo.categoryCount;
            devMenu.timer = (devMenu.timer + 1) & 7;
            if (devMenu.option >= devMenu.scroll) {
                if (devMenu.option > devMenu.scroll + 7) {
                    devMenu.scroll = devMenu.option - 7;
                }
            }
            else {
                devMenu.scroll = devMenu.option;
            }
        }
    }

    if (controller[CONT_P1].keyDown.press) {
        if (++devMenu.option == sceneInfo.categoryCount)
            devMenu.option = 0;
        if (devMenu.option >= devMenu.scroll) {
            if (devMenu.option > devMenu.scroll + 7) {
                devMenu.scroll = devMenu.option - 7;
            }
        }
        else {
            devMenu.scroll = devMenu.option;
        }
        devMenu.timer = 1;
    }
    else if (controller[CONT_P1].keyDown.down) {
        if (devMenu.timer) {
            devMenu.timer = (devMenu.timer + 1) & 7;
            if (devMenu.option >= devMenu.scroll) {
                if (devMenu.option > devMenu.scroll + 7) {
                    devMenu.scroll = devMenu.option - 7;
                }
            }
            else {
                devMenu.scroll = devMenu.option;
            }
        }
        else {
            if (++devMenu.option == sceneInfo.categoryCount)
                devMenu.option = 0;
            devMenu.timer = (devMenu.timer + 1) & 7;
            if (devMenu.option >= devMenu.scroll) {
                if (devMenu.option > devMenu.scroll + 7) {
                    devMenu.scroll = devMenu.option - 7;
                }
            }
            else {
                devMenu.scroll = devMenu.option;
            }
        }
    }

    bool32 confirm = controller[CONT_P1].keyA.press;
#if RETRO_REV02
    if (RSDK::SKU::userCore->GetConfirmButtonFlip())
#else
    if (RSDK::SKU::GetConfirmButtonFlip())
#endif
        confirm = controller[CONT_P1].keyB.press;

    if (controller[CONT_P1].keyStart.press || confirm) {
        if (sceneInfo.listCategory[devMenu.option].sceneCount) {
            devMenu.state   = DevMenu_SceneSel;
            devMenu.listPos = devMenu.option;
            devMenu.scroll  = 0;
            devMenu.option  = 0;
        }
    }
#if !RETRO_USE_ORIGINAL_CODE
    else if (controller[CONT_P1].keyB.press) {
        devMenu.state   = DevMenu_MainMenu;
        devMenu.listPos = 0;
        devMenu.scroll  = 0;
        devMenu.option  = 2;
    }
#endif
}
void DevMenu_SceneSel()
{
    int dy = currentScreen->center.y;
    DrawRectangle(currentScreen->center.x - 128, dy - 84, 256, 48, 128, 255, INK_NONE, true);
    dy -= 68;
    DrawDevText("SELECT STAGE SCENE", currentScreen->center.x, dy, ALIGN_CENTER, 0xF0F0F0);
    DrawRectangle(currentScreen->center.x - 128, dy + 36, 256, 72, 128, 255, INK_NONE, true);

    uint optionColours[8];
    optionColours[0]                               = 0x808090;
    optionColours[1]                               = 0x808090;
    optionColours[2]                               = 0x808090;
    optionColours[3]                               = 0x808090;
    optionColours[4]                               = 0x808090;
    optionColours[5]                               = 0x808090;
    optionColours[6]                               = 0x808090;
    optionColours[7]                               = 0x808090;
    optionColours[devMenu.option - devMenu.scroll] = 0xF0F0F0;

    int y               = dy + 40;
    SceneListInfo *list = &sceneInfo.listCategory[devMenu.listPos];
    int off             = list->sceneOffsetStart;
    for (int i = 0; i < 8; ++i) {
        if (devMenu.scroll + i < list->sceneCount) {
            DrawDevText(sceneInfo.listData[off + (devMenu.scroll + i)].name, currentScreen->center.x + 96, y, ALIGN_RIGHT, optionColours[i]);
            y += 8;
            devMenu.scroll = devMenu.scroll;
        }
    }

#if !RETRO_USE_ORIGINAL_CODE
    DevMenu_HandleTouchControls();
#endif

    if (controller[CONT_P1].keyUp.press) {
        devMenu.option--;
        if (off + devMenu.option < list->sceneOffsetStart) {
            devMenu.option = list->sceneCount - 1;
        }

        if (devMenu.option >= devMenu.scroll) {
            if (devMenu.option > devMenu.scroll + 7) {
                devMenu.scroll = devMenu.option - 7;
            }
        }
        else {
            devMenu.scroll = devMenu.option;
        }
        devMenu.timer = 1;
    }
    else if (controller[CONT_P1].keyUp.down) {
        if (!devMenu.timer) {
            devMenu.option--;
            if (off + devMenu.option < list->sceneOffsetStart) {
                devMenu.option = list->sceneCount - 1;
            }
        }

        devMenu.timer = (devMenu.timer + 1) & 7;
        if (devMenu.option >= devMenu.scroll) {
            if (devMenu.option > devMenu.scroll + 7) {
                devMenu.scroll = devMenu.option - 7;
            }
        }
        else {
            devMenu.scroll = devMenu.option;
        }
    }

    if (controller[CONT_P1].keyDown.press) {
        devMenu.option++;
        if (devMenu.option >= list->sceneCount) {
            devMenu.option = 0;
        }

        if (devMenu.option >= devMenu.scroll) {
            if (devMenu.option > devMenu.scroll + 7) {
                devMenu.scroll = devMenu.option - 7;
            }
        }
        else {
            devMenu.scroll = devMenu.option;
        }
        devMenu.timer = 1;
    }
    else if (controller[CONT_P1].keyDown.down) {
        if (!devMenu.timer) {
            devMenu.option++;
            if (devMenu.option >= list->sceneCount) {
                devMenu.option = 0;
            }
        }

        devMenu.timer = (devMenu.timer + 1) & 7;
        if (devMenu.option >= devMenu.scroll) {
            if (devMenu.option > devMenu.scroll + 7) {
                devMenu.scroll = devMenu.option - 7;
            }
        }
        else {
            devMenu.scroll = devMenu.option;
        }
    }

    bool32 confirm = controller[CONT_P1].keyA.press;
#if RETRO_REV02
    if (RSDK::SKU::userCore->GetConfirmButtonFlip())
#else
    if (RSDK::SKU::GetConfirmButtonFlip())
#endif
        confirm = controller[CONT_P1].keyB.press;

    if (controller[CONT_P1].keyStart.press || confirm) {
        sceneInfo.activeCategory = devMenu.listPos;
        sceneInfo.listPos        = devMenu.option + list->sceneOffsetStart;
        sceneInfo.state          = ENGINESTATE_LOAD;
    }
#if !RETRO_USE_ORIGINAL_CODE
    else if (controller[CONT_P1].keyB.press) {
        devMenu.state   = DevMenu_ListSel;
        devMenu.scroll  = 0;
        devMenu.option  = 0;
        devMenu.listPos = 0;
    }
#endif
}
void DevMenu_Options()
{
    int dy = currentScreen->center.y;
    DrawRectangle(currentScreen->center.x - 128, dy - 84, 256, 48, 128, 255, INK_NONE, true);
    dy -= 68;
    DrawDevText("OPTIONS", currentScreen->center.x, dy, ALIGN_CENTER, 0xF0F0F0);
    dy += 44;
    DrawRectangle(currentScreen->center.x - 128, dy - 8, 256, 72, 128, 255, INK_NONE, true);

    const byte optionCount = RETRO_REV02 ? 5 : 4;

    uint optionColours[optionCount];
    optionColours[0] = 0x808090;
    optionColours[1] = 0x808090;
    optionColours[2] = 0x808090;
    optionColours[3] = 0x808090;
#if RETRO_REV02
    optionColours[4] = 0x808090;
#endif
    optionColours[devMenu.option] = 0xF0F0F0;

    DrawDevText("Video Settings", currentScreen->center.x, dy, ALIGN_CENTER, optionColours[0]);
    dy += 12;
    DrawDevText("Audio Settings", currentScreen->center.x, dy, ALIGN_CENTER, optionColours[1]);
    dy += 12;
    DrawDevText("Configure Input", currentScreen->center.x, dy, ALIGN_CENTER, optionColours[2]);
#if RETRO_REV02
    dy += 12;
    DrawDevText("Debug Flags", currentScreen->center.x, dy, ALIGN_CENTER, optionColours[3]);
#endif
    DrawDevText("Back", currentScreen->center.x, dy + 12, ALIGN_CENTER, optionColours[optionCount - 1]);

#if !RETRO_USE_ORIGINAL_CODE
    DevMenu_HandleTouchControls();
#endif

    if (controller[CONT_P1].keyUp.press) {
        devMenu.option--;
        if (devMenu.option < 0) {
            devMenu.option = optionCount - 1;
        }

        devMenu.timer = 1;
    }
    else if (controller[CONT_P1].keyUp.down) {
        if (!devMenu.timer) {
            devMenu.option--;
            if (devMenu.option < 0) {
                devMenu.option = optionCount - 1;
            }
        }

        devMenu.timer = (devMenu.timer + 1) & 7;
    }

    if (controller[CONT_P1].keyDown.press) {
        devMenu.option++;
        if (devMenu.option >= optionCount) {
            devMenu.option = 0;
        }

        devMenu.timer = 1;
    }
    else if (controller[CONT_P1].keyDown.down) {
        if (!devMenu.timer) {
            devMenu.option++;
            if (devMenu.option >= optionCount) {
                devMenu.option = 0;
            }
        }

        devMenu.timer = (devMenu.timer + 1) & 7;
    }

    bool32 confirm = controller[CONT_P1].keyA.press;
#if RETRO_REV02
    if (RSDK::SKU::userCore->GetConfirmButtonFlip())
#else
    if (RSDK::SKU::GetConfirmButtonFlip())
#endif
        confirm = controller[CONT_P1].keyB.press;

    if (controller[CONT_P1].keyStart.press || confirm) {
        switch (devMenu.option) {
            case 0: {
                devMenu.windowed = !engine.isWindowed;
                devMenu.winScale = (engine.windowWidth / pixWidth) - 1;
                int aspect       = (int)((engine.windowWidth / (float)engine.windowHeight) * (float)SCREEN_YSIZE) >> 3;
                switch (aspect) {
                    case 40: devMenu.winAspect = 0; break;
                    case 45: devMenu.winAspect = 1; break;
                    case 48: devMenu.winAspect = 2; break;
                    case 50: devMenu.winAspect = 3; break;
                    default: devMenu.winAspect = aspect != 53 ? 0 : 4; break;
                }
                devMenu.state  = DevMenu_VideoOptions;
                devMenu.option = 0;
            } break;
            case 1:
                devMenu.state  = DevMenu_AudioOptions;
                devMenu.option = 0;
                break;
            case 2:
                devMenu.state  = DevMenu_InputOptions;
                devMenu.option = 0;
                break;
#if RETRO_REV02
            case 3:
                devMenu.state  = DevMenu_DebugOptions;
                devMenu.option = 0;
                break;
            case 4:
#else
            case 3:
#endif
                devMenu.state  = DevMenu_MainMenu;
                devMenu.option = 0;
                break;
        }
    }
#if !RETRO_USE_ORIGINAL_CODE
    else if (controller[CONT_P1].keyB.press) {
        devMenu.state   = DevMenu_MainMenu;
        devMenu.listPos = 0;
        devMenu.scroll  = 0;
        devMenu.option  = 3;
    }
#endif
}
void DevMenu_VideoOptions()
{
    int dy = currentScreen->center.y;
    DrawRectangle(currentScreen->center.x - 128, dy - 84, 256, 48, 128, 255, INK_NONE, true);
    dy -= 68;
    DrawDevText("VIDEO SETTINGS", currentScreen->center.x, dy, ALIGN_CENTER, 0xF0F0F0);
    dy += 44;
    DrawRectangle(currentScreen->center.x - 128, dy - 8, 256, 72, 128, 255, INK_NONE, true);

    uint optionColours[6];
    optionColours[0]              = 0x808090;
    optionColours[1]              = 0x808090;
    optionColours[2]              = 0x808090;
    optionColours[3]              = 0x808090;
    optionColours[4]              = 0x808090;
    optionColours[5]              = 0x808090;
    optionColours[devMenu.option] = 0xF0F0F0;

    DrawDevText("Window Size:", currentScreen->center.x - 96, dy, 0, optionColours[0]);

    const char *winScale = "unknown";
    switch (devMenu.winScale) {
        case 0: winScale = "1x"; break;
        case 1: winScale = "2x"; break;
        case 2: winScale = "3x"; break;
        case 3: winScale = "4x"; break;
        default: break;
    }
    DrawDevText(winScale, currentScreen->center.x + 80, dy, ALIGN_CENTER, 0xF0F080);
    dy += 8;
    DrawDevText("Window Aspect:", currentScreen->center.x - 96, dy, 0, optionColours[1]);

    const char *winAspect = "unknown";
    switch (devMenu.winAspect) {
        case 0: winAspect = "4:3"; break;
        case 1: winAspect = "3:2"; break;
        case 2: winAspect = "16:10"; break;
        case 3: winAspect = "5:3"; break;
        case 4: winAspect = "16:9"; break;
        default: break;
    }
    DrawDevText(winAspect, currentScreen->center.x + 80, dy, ALIGN_CENTER, 0xF0F080);
    dy += 8;
    DrawDevText("Fullscreen:", currentScreen->center.x - 96, dy, 0, optionColours[2]);
    const char *fsOpt = "NO";
    if (devMenu.windowed != 1)
        fsOpt = "YES";
    DrawDevText(fsOpt, currentScreen->center.x + 80, dy, ALIGN_CENTER, 0xF0F080);
    dy += 8;
    DrawDevText("Screen Shader:", currentScreen->center.x - 96, dy, ALIGN_LEFT, optionColours[3]);
    DrawDevText(shaderList[engine.shaderID].name, currentScreen->center.x + 80, dy, ALIGN_CENTER, 0xF0F080);
    dy += 16;
    DrawDevText("Confirm", currentScreen->center.x, dy, ALIGN_CENTER, optionColours[4]);
    DrawDevText("Cancel", currentScreen->center.x, dy + 8, ALIGN_CENTER, optionColours[5]);

#if !RETRO_USE_ORIGINAL_CODE
    DevMenu_HandleTouchControls();
#endif

    if (controller[CONT_P1].keyUp.press) {
        devMenu.option--;
        if (devMenu.option < 0) {
            devMenu.option = 5;
        }

        devMenu.timer = 1;
    }
    else if (controller[CONT_P1].keyUp.down) {
        if (!devMenu.timer) {
            devMenu.option--;
            if (devMenu.option < 0) {
                devMenu.option = 5;
            }
        }

        devMenu.timer = (devMenu.timer + 1) & 7;
    }

    if (controller[CONT_P1].keyDown.press) {
        devMenu.option++;
        if (devMenu.option > 5) {
            devMenu.option = 0;
        }

        devMenu.timer = 1;
    }
    else if (controller[CONT_P1].keyDown.down) {
        if (!devMenu.timer) {
            devMenu.option++;
            if (devMenu.option > 5) {
                devMenu.option = 0;
            }
        }

        devMenu.timer = (devMenu.timer + 1) & 7;
    }

    switch (devMenu.option) {
        case 0: // scale
            if (controller[CONT_P1].keyLeft.press) {
                devMenu.winScale = (devMenu.winScale - 1) & 3;
                RSDK::SKU::settingsChanged = true;
            }
            else if (controller[CONT_P1].keyRight.press) {
                devMenu.winScale = (devMenu.winScale + 1) & 3;
                RSDK::SKU::settingsChanged = true;
            }
            break;
        case 1: // aspect
            if (controller[CONT_P1].keyLeft.press) {
                devMenu.winAspect--;
                RSDK::SKU::settingsChanged = true;
            }
            else if (controller[CONT_P1].keyRight.press) {
                devMenu.winAspect++;
                RSDK::SKU::settingsChanged = true;
            }

            if (devMenu.winAspect > 4) 
                devMenu.winAspect = 0;
            else if (devMenu.winAspect < 0) 
                devMenu.winAspect = 4;
            break;
        case 2: // fullscreen
            if (controller[CONT_P1].keyLeft.press || controller[CONT_P1].keyRight.press) {
                devMenu.windowed ^= 1;
                RSDK::SKU::settingsChanged = true;
            }
            break;
        case 3: // screenShader
            if (controller[CONT_P1].keyLeft.press) {
                engine.shaderID--;
                RSDK::SKU::settingsChanged = true;
            }
            else if (controller[CONT_P1].keyRight.press) {
                engine.shaderID++;
                RSDK::SKU::settingsChanged = true;
            }

            if (engine.shaderID > SHADER_CRT_YEE64) {
                engine.shaderID = SHADER_NONE;
            }
            else if (engine.shaderID < SHADER_NONE) {
                engine.shaderID = SHADER_CRT_YEE64;
            }
            break;
        case 4: // confirm
        {
            bool32 confirm = controller[CONT_P1].keyA.press;
#if RETRO_REV02
            if (RSDK::SKU::userCore->GetConfirmButtonFlip())
#else
            if (RSDK::SKU::GetConfirmButtonFlip())
#endif
                confirm = controller[CONT_P1].keyB.press;

            if (controller[CONT_P1].keyStart.press || confirm) {
                // do confirm
                engine.isWindowed    = !devMenu.windowed;
                shaderList[0].linear = !devMenu.windowed;
                if (!devMenu.winScale)
                    engine.shaderID = SHADER_NONE;

                int width = 0;
                switch (devMenu.winAspect) {
                    case 0: width = 3 - (float)((float)engine.gameHeight * -1.3333334); break;
                    case 1: width = 3 - (float)((float)engine.gameHeight * -1.5); break;
                    case 2: width = 3 - (float)((float)engine.gameHeight * -1.6); break;
                    case 3: width = 3 - (float)((float)engine.gameHeight * -1.6666666); break;
                    case 4: width = 3 - (float)((float)engine.gameHeight * -1.7777778); break;
                    default: width = engine.windowWidth; break;
                }
                width &= 0x7FF8;
                if (width > 424)
                    width = 424;
                engine.windowWidth  = width * (devMenu.winScale + 1);
                engine.windowHeight = engine.gameHeight * (devMenu.winScale + 1);
                UpdateWindow();

                devMenu.state  = DevMenu_Options;
                devMenu.option = 0;
            }
            break;
        }
        case 5: // cancel
        {
            bool32 confirm = controller[CONT_P1].keyA.press;
#if RETRO_REV02
            if (RSDK::SKU::userCore->GetConfirmButtonFlip())
#else
            if (RSDK::SKU::GetConfirmButtonFlip())
#endif
                confirm = controller[CONT_P1].keyB.press;

            if (controller[CONT_P1].keyStart.press || confirm) {
                devMenu.state  = DevMenu_Options;
                devMenu.option = 0;
            }
            break;
        }
    }

#if !RETRO_USE_ORIGINAL_CODE
    if (controller[CONT_P1].keyB.press) {
        devMenu.state   = DevMenu_Options;
        devMenu.option  = 0;
    }
#endif
}
void DevMenu_AudioOptions()
{
    int dy = currentScreen->center.y;
    DrawRectangle(currentScreen->center.x - 128, dy - 84, 256, 48, 128, 255, INK_NONE, true);
    dy -= 68;
    DrawDevText("AUDIO SETTINGS", currentScreen->center.x, dy, ALIGN_CENTER, 0xF0F0F0);
    dy += 44;
    DrawRectangle(currentScreen->center.x - 128, dy - 8, 256, 72, 128, 255, INK_NONE, true);

    uint optionColours[4];
    optionColours[0]              = 0x808090;
    optionColours[1]              = 0x808090;
    optionColours[2]              = 0x808090;
    optionColours[3]              = 0x808090;
    optionColours[devMenu.option] = 0xF0F0F0;

    DrawDevText("Streams Enabled:", currentScreen->center.x - 96, dy, 0, optionColours[0]);

    const char *strmEnabled = "YES";
    if (!engine.streamsEnabled)
        strmEnabled = "NO";
    DrawDevText(strmEnabled, currentScreen->center.x + 80, dy, ALIGN_CENTER, 15790208);
    dy += 16;

    DrawDevText("Streams Vol:", currentScreen->center.x - 96, dy, 0, optionColours[1]);
    DrawRectangle(currentScreen->center.x + 8, dy, 112, 8, 0, 255, INK_NONE, true);
    DrawRectangle(currentScreen->center.x + 9, dy + 1, (int)(engine.streamVolume * 110.0), 6, 0xF0F0F0, 255, INK_NONE, true);
    dy += 16;

    DrawDevText("SoundFX Vol:", currentScreen->center.x - 96, dy, 0, optionColours[2]);
    DrawRectangle(currentScreen->center.x + 8, dy, 112, 8, 0, 255, INK_NONE, true);
    DrawRectangle(currentScreen->center.x + 9, dy + 1, (int)(engine.soundFXVolume * 110.0), 6, 0xF0F0F0, 255, INK_NONE, true);
    DrawDevText("Back", currentScreen->center.x, dy + 16, ALIGN_CENTER, optionColours[3]);

#if !RETRO_USE_ORIGINAL_CODE
    DevMenu_HandleTouchControls();
#endif

    if (controller[CONT_P1].keyUp.press) {
        devMenu.option--;
        if (devMenu.option < 0) {
            devMenu.option = 3;
        }

        devMenu.timer = 1;
    }
    else if (controller[CONT_P1].keyUp.down) {
        if (!devMenu.timer) {
            devMenu.option--;
            if (devMenu.option < 0) {
                devMenu.option = 3;
            }
        }

        devMenu.timer = (devMenu.timer + 1) & 7;
    }

    if (controller[CONT_P1].keyDown.press) {
        devMenu.option++;
        if (devMenu.option > 3) {
            devMenu.option = 0;
        }

        devMenu.timer = 1;
    }
    else if (controller[CONT_P1].keyDown.down) {
        if (!devMenu.timer) {
            devMenu.option++;
            if (devMenu.option > 3) {
                devMenu.option = 0;
            }
        }

        devMenu.timer = (devMenu.timer + 1) & 7;
    }

    switch (devMenu.option) {
        case 0:
            if (controller[CONT_P1].keyLeft.press || controller[CONT_P1].keyRight.press) {
                engine.streamsEnabled ^= 1;
                RSDK::SKU::settingsChanged = true;
            }
            break;
        case 1:
            if (controller[CONT_P1].keyLeft.down) {
                engine.streamVolume -= 0.015625;
                if (engine.streamVolume < 0.0)
                    engine.streamVolume = 0.0;
                RSDK::SKU::settingsChanged = true;
            }
            else {
                if (controller[CONT_P1].keyRight.down) {
                    engine.streamVolume += 0.015625;
                    if (engine.streamVolume > 1.0)
                        engine.streamVolume = 1.0;
                    RSDK::SKU::settingsChanged = true;
                }
            }
            break;
        case 2:
            if (controller[CONT_P1].keyLeft.down) {
                engine.soundFXVolume -= 0.015625;
                if (engine.soundFXVolume < 0.0)
                    engine.soundFXVolume = 0.0;
                RSDK::SKU::settingsChanged = true;
            }
            else {
                if (controller[CONT_P1].keyRight.down) {
                    engine.soundFXVolume += 0.015625;
                    if (engine.soundFXVolume > 1.0)
                        engine.soundFXVolume = 1.0;
                    RSDK::SKU::settingsChanged = true;
                }
            }
            break;
        case 3: {
            bool32 confirm = controller[CONT_P1].keyA.press;
#if RETRO_REV02
            if (RSDK::SKU::userCore->GetConfirmButtonFlip())
#else
            if (RSDK::SKU::GetConfirmButtonFlip())
#endif
                confirm = controller[CONT_P1].keyB.press;

            if (controller[CONT_P1].keyStart.press || confirm) {
                devMenu.state  = DevMenu_Options;
                devMenu.option = 1;
            }
            break;
        }
    }

#if !RETRO_USE_ORIGINAL_CODE
    if (controller[CONT_P1].keyB.press) {
        devMenu.state   = DevMenu_Options;
        devMenu.option  = 1;
    }
#endif
}
void DevMenu_InputOptions()
{
    int dy = currentScreen->center.y;
    DrawRectangle(currentScreen->center.x - 128, dy - 84, 256, 48, 128, 255, INK_NONE, true);
    dy -= 68;
    DrawDevText("CONFIGURE INPUT", currentScreen->center.x, dy, ALIGN_CENTER, 0xF0F0F0);
    dy += 44;
    DrawRectangle(currentScreen->center.x - 128, dy - 8, 256, 72, 128, 255, INK_NONE, true);

    uint optionColours[6];
    optionColours[0]              = 0x808090;
    optionColours[1]              = 0x808090;
    optionColours[2]              = 0x808090;
    optionColours[3]              = 0x808090;
    optionColours[4]              = 0x808090;
    optionColours[devMenu.option] = 0xF0F0F0;

    DrawDevText("Set Keys For Input 1", currentScreen->center.x, dy, ALIGN_CENTER, optionColours[0]);
    dy += 10;
    DrawDevText("Set Keys For Input 2", currentScreen->center.x, dy, ALIGN_CENTER, optionColours[1]);
    dy += 10;
    DrawDevText("Set Keys For Input 3", currentScreen->center.x, dy, ALIGN_CENTER, optionColours[2]);
    dy += 10;
    DrawDevText("Set Keys For Input 4", currentScreen->center.x, dy, ALIGN_CENTER, optionColours[3]);
    DrawDevText("Back", currentScreen->center.x, dy + 18, ALIGN_CENTER, optionColours[4]);

#if !RETRO_USE_ORIGINAL_CODE
    DevMenu_HandleTouchControls();
#endif

    if (controller[CONT_P1].keyUp.press) {
        devMenu.option--;
        if (devMenu.option < 0) {
            devMenu.option = 4;
        }

        devMenu.timer = 1;
    }
    else if (controller[CONT_P1].keyUp.down) {
        if (!devMenu.timer) {
            devMenu.option--;
            if (devMenu.option < 0) {
                devMenu.option = 4;
            }
        }

        devMenu.timer = (devMenu.timer + 1) & 7;
    }

    if (controller[CONT_P1].keyDown.press) {
        devMenu.option++;
        if (devMenu.option > 4) {
            devMenu.option = 0;
        }

        devMenu.timer = 1;
    }
    else if (controller[CONT_P1].keyDown.down) {
        if (!devMenu.timer) {
            devMenu.option++;
            if (devMenu.option > 4) {
                devMenu.option = 0;
            }
        }

        devMenu.timer = (devMenu.timer + 1) & 7;
    }

    bool32 confirm = controller[CONT_P1].keyA.press;
#if RETRO_REV02
    if (RSDK::SKU::userCore->GetConfirmButtonFlip())
#else
    if (RSDK::SKU::GetConfirmButtonFlip())
#endif
        confirm = controller[CONT_P1].keyB.press;

    if (controller[CONT_P1].keyStart.press || confirm) {
        if (devMenu.option == 4) {
            devMenu.state  = DevMenu_Options;
            devMenu.option = 3;
        }
        else {
            devMenu.state   = DevMenu_MappingsOptions;
            devMenu.scroll  = 0;
            RSDK::SKU::settingsChanged = true;
        }
    }

#if !RETRO_USE_ORIGINAL_CODE
    if (controller[CONT_P1].keyB.press) {
        devMenu.state   = DevMenu_Options;
        devMenu.option  = 2;
    }
#endif
}
void DevMenu_MappingsOptions()
{
#if !RETRO_USE_ORIGINAL_CODE
    DevMenu_HandleTouchControls();
#endif

    int dy = currentScreen->center.y;
    DrawRectangle(currentScreen->center.x - 128, dy - 84, 256, 48, 128, 255, INK_NONE, true);
    dy -= 68;
    DrawDevText("SET KEY BINDING", currentScreen->center.x, dy, ALIGN_CENTER, 0xF0F0F0);
    dy += 44;
    DrawRectangle(currentScreen->center.x - 128, dy - 8, 256, 72, 128, 255, INK_NONE, true);

    int controllerID     = devMenu.option + 1;
    switch (devMenu.scroll) {
        case 0:
            DrawDevText("Press Key For UP", currentScreen->center.x, dy, ALIGN_CENTER, 0xF0F080);
            if (controller[controllerID].keyUp.keyMap != -1) {
                controller[controllerID].keyDown.keyMap = -1;
                ++devMenu.scroll;
            }
            break;
        case 1:
            DrawDevText("Press Key For DOWN", currentScreen->center.x, dy, ALIGN_CENTER, 0xF0F080);
            if (controller[controllerID].keyDown.keyMap != -1) {
                controller[controllerID].keyLeft.keyMap = -1;
                ++devMenu.scroll;
            }
            break;
        case 2:
            DrawDevText("Press Key For LEFT", currentScreen->center.x, dy, ALIGN_CENTER, 0xF0F080);
            if (controller[controllerID].keyLeft.keyMap != -1) {
                controller[controllerID].keyRight.keyMap = -1;
                ++devMenu.scroll;
            }
            break;
        case 3:
            DrawDevText("Press Key For RIGHT", currentScreen->center.x, dy, ALIGN_CENTER, 0xF0F080);
            if (controller[controllerID].keyRight.keyMap != -1) {
                controller[controllerID].keyA.keyMap = -1;
                ++devMenu.scroll;
            }
            break;
        case 4:
            DrawDevText("Press Key For BUTTON A", currentScreen->center.x, dy, ALIGN_CENTER, 0xF0F080);
            if (controller[controllerID].keyA.keyMap != -1) {
                controller[controllerID].keyB.keyMap = -1;
                ++devMenu.scroll;
            }
            break;
        case 5:
            DrawDevText("Press Key For BUTTON B", currentScreen->center.x, dy, ALIGN_CENTER, 0xF0F080);
            if (controller[controllerID].keyB.keyMap != -1) {
                controller[controllerID].keyC.keyMap = -1;
                ++devMenu.scroll;
            }
            break;
        case 6:
            DrawDevText("Press Key For BUTTON C", currentScreen->center.x, dy, ALIGN_CENTER, 0xF0F080);
            if (controller[controllerID].keyC.keyMap != -1) {
                controller[controllerID].keyX.keyMap = -1;
                ++devMenu.scroll;
            }
            break;
        case 7:
            DrawDevText("Press Key For BUTTON X", currentScreen->center.x, dy, ALIGN_CENTER, 0xF0F080);
            if (controller[controllerID].keyX.keyMap != -1) {
                controller[controllerID].keyY.keyMap = -1;
                ++devMenu.scroll;
            }
            break;
        case 8:
            DrawDevText("Press Key For BUTTON Y", currentScreen->center.x, dy, ALIGN_CENTER, 0xF0F080);
            if (controller[controllerID].keyY.keyMap != -1) {
                controller[controllerID].keyZ.keyMap = -1;
                ++devMenu.scroll;
            }
            break;
        case 9:
            DrawDevText("Press Key For BUTTON Z", currentScreen->center.x, dy, ALIGN_CENTER, 0xF0F080);
            if (controller[controllerID].keyZ.keyMap != -1) {
                controller[controllerID].keyStart.keyMap = -1;
                ++devMenu.scroll;
            }
            break;
        case 10:
            DrawDevText("Press Key For START", currentScreen->center.x, dy, ALIGN_CENTER, 0xF0F080);
            if (controller[controllerID].keyStart.keyMap != -1) {
                controller[controllerID].keySelect.keyMap = -1;
                ++devMenu.scroll;
            }
            break;
        case 11:
            DrawDevText("Press Key For SELECT", currentScreen->center.x, dy, ALIGN_CENTER, 0xF0F080);
            if (controller[controllerID].keySelect.keyMap != -1)
                devMenu.state = DevMenu_InputOptions;
            break;
        default: break;
    }

#if !RETRO_USE_ORIGINAL_CODE
    if (controller[CONT_P1].keyB.press) {
        devMenu.state   = DevMenu_Options;
        devMenu.option  = 3;
    }
#endif 
}
#if RETRO_REV02
void DevMenu_DebugOptions()
{
    int dy = currentScreen->center.y;
    DrawRectangle(currentScreen->center.x - 128, dy - 84, 256, 48, 128, 255, INK_NONE, true);
    dy -= 68;
    DrawDevText("CONFIGURE DEBUG FLAGS", currentScreen->center.x, dy, ALIGN_CENTER, 0xF0F0F0);
    dy += 40;
    DrawRectangle(currentScreen->center.x - 128, dy - 4, 256, 72, 128, 255, INK_NONE, true);
    uint optionColours[8];
    optionColours[0]                               = 0x808090;
    optionColours[1]                               = 0x808090;
    optionColours[2]                               = 0x808090;
    optionColours[3]                               = 0x808090;
    optionColours[4]                               = 0x808090;
    optionColours[5]                               = 0x808090;
    optionColours[6]                               = 0x808090;
    optionColours[7]                               = 0x808090;
    optionColours[devMenu.option - devMenu.scroll] = 0xF0F0F0;

#if !RETRO_USE_ORIGINAL_CODE
    DevMenu_HandleTouchControls();
#endif

    bool32 confirm = controller[CONT_P1].keyA.press;
    if (RSDK::SKU::userCore->GetConfirmButtonFlip())
        confirm = controller[CONT_P1].keyB.press;

    confirm |= controller[CONT_P1].keyStart.press;

    for (int i = 0; i < 8; ++i) {
        if (devMenu.scroll + i < debugValCnt) {
            DebugValueInfo *val = &debugValues[devMenu.scroll + i];
            DrawDevText(val->name, currentScreen->center.x - 96, dy, ALIGN_LEFT, optionColours[i]);
            if (!val->value) {
                DrawDevText("--------", currentScreen->center.x + 96, dy, ALIGN_RIGHT, 0xF0F080);
            }
            else {
                char valBuf[0x10];
                strcpy(valBuf, "--------");
                switch (val->valByteCnt) {
                    default: DrawDevText("--------", currentScreen->center.x + 96, dy, ALIGN_RIGHT, 0xF0F080); break;
                    case sizeof(sbyte): {
                        sbyte *v = (sbyte *)val->value;
                        if (val->type == 2) {
                            valBuf[0] = ' ';
                            if (*v > 0x7F)
                                valBuf[0] = '-';
                            *v &= 0x7FFF;
                        }
                        else if (!val->type) {
                            valBuf[0] = 'Y';
                            if (!*v)
                                valBuf[0] = 'N';
                            valBuf[1] = 0;
                        }
                        else {
                            valBuf[0] = ' ';
                        }
                        break;
                    }
                    case sizeof(short): {
                        short *v = (short *)val->value;
                        if (val->type == 2) {
                            valBuf[0] = ' ';
                            if (*v > 0x7FFF)
                                valBuf[0] = '-';
                            *v &= 0x7FFF;
                        }
                        else if (!val->type) {
                            short *v  = (short *)val->value;
                            valBuf[0] = 'Y';
                            if (!*v)
                                valBuf[0] = 'N';
                            valBuf[1] = 0;
                        }
                        else {
                            valBuf[0] = ' ';
                        }
                        break;
                    }
                    case sizeof(int): {
                        int *v = (int *)val->value;
                        if (val->type == 2) {
                            valBuf[0] = ' ';
                            if (*v > 0x7FFFFFFF)
                                valBuf[0] = '-';
                            *v &= 0x7FFFFFFF;
                        }
                        else if (!val->type) {
                            valBuf[0] = 'Y';
                            if (!*v)
                                valBuf[0] = 'N';
                            valBuf[1] = 0;
                        }
                        else {
                            valBuf[0] = ' ';
                        }
                        break;
                    }
                }

                if (val->type) {
                    if (2 * val->valByteCnt) {
                        char *bufPtr                      = &valBuf[2 * val->valByteCnt];
                        valBuf[(2 * val->valByteCnt) + 1] = 0;

                        switch (val->valByteCnt) {
                            default: break;
                            case sizeof(sbyte): {
                                sbyte *value = (sbyte *)val->value;
                                for (int v = 0; v < 2 * val->valByteCnt; ++v) {
                                    *bufPtr-- = ((v & 0xF) > 9 ? '7' : '0') + ((*value >> 4 * v) & 0xF);
                                }
                                break;
                            }
                            case sizeof(short): {
                                short *value = (short *)val->value;
                                for (int v = 0; v < 2 * val->valByteCnt; ++v) {
                                    *bufPtr-- = ((v & 0xF) > 9 ? '7' : '0') + ((*value >> 4 * v) & 0xF);
                                }
                                break;
                            }
                            case sizeof(int): {
                                int *value = (int *)val->value;
                                for (int v = 0; v < 2 * val->valByteCnt; ++v) {
                                    *bufPtr-- = ((v & 0xF) > 9 ? '7' : '0') + ((*value >> 4 * v) & 0xF);
                                }
                                break;
                            }
                        }
                    }
                }
                DrawDevText(valBuf, currentScreen->center.x + 96, dy, ALIGN_CENTER, optionColours[i]);
            }
            dy += 8;
        }
        else {
            DrawDevText("Back", currentScreen->center.x, dy, ALIGN_CENTER, optionColours[i]);
        }
    }

    if (controller[CONT_P1].keyUp.press) {
        devMenu.option--;
        if (devMenu.option < 0) {
            devMenu.option = debugValCnt;
        }

        if (devMenu.option >= devMenu.scroll) {
            if (devMenu.option > devMenu.scroll + 7) {
                devMenu.scroll = devMenu.option - 7;
            }
        }
        else {
            devMenu.scroll = devMenu.option;
        }
        devMenu.timer = 1;
    }
    else if (controller[CONT_P1].keyUp.down) {
        if (!devMenu.timer) {
            devMenu.option--;
            if (devMenu.option < 0) {
                devMenu.option = debugValCnt;
            }
        }

        devMenu.timer = (devMenu.timer + 1) & 7;
        if (devMenu.option >= devMenu.scroll) {
            if (devMenu.option > devMenu.scroll + 7) {
                devMenu.scroll = devMenu.option - 7;
            }
        }
        else {
            devMenu.scroll = devMenu.option;
        }
    }

    if (controller[CONT_P1].keyDown.press) {
        devMenu.option++;
        if (devMenu.option > debugValCnt) {
            devMenu.option = 0;
        }

        if (devMenu.option >= devMenu.scroll) {
            if (devMenu.option > devMenu.scroll + 7) {
                devMenu.scroll = devMenu.option - 7;
            }
        }
        else {
            devMenu.scroll = devMenu.option;
        }
        devMenu.timer = 1;
    }
    else if (controller[CONT_P1].keyDown.down) {
        if (!devMenu.timer) {
            devMenu.option++;
            if (devMenu.option >= debugValCnt) {
                devMenu.option = 0;
            }
        }

        devMenu.timer = (devMenu.timer + 1) & 7;
        if (devMenu.option >= devMenu.scroll) {
            if (devMenu.option > devMenu.scroll + 7) {
                devMenu.scroll = devMenu.option - 7;
            }
        }
        else {
            devMenu.scroll = devMenu.option;
        }
    }

    if (devMenu.option < debugValCnt) {
        DebugValueInfo *val = &debugValues[devMenu.option];
        switch (val->valByteCnt) {
            default: DrawDevText("--------", currentScreen->center.x + 96, dy, ALIGN_RIGHT, 0xF0F080); break;
            case sizeof(sbyte): {
                sbyte *v = (sbyte *)val->value;
                if (controller[CONT_P1].keyLeft.press) {
                    if (!val->type) {
                        *v ^= 1;
                    }
                    else {
                        if (*v - 1 >= val->min) {
                            *v -= 1;
                        }
                    }
                }

                if (controller[CONT_P1].keyRight.press) {
                    if (!val->type) {
                        *v ^= 1;
                    }
                    else {
                        if (*v + 1 <= val->max) {
                            *v += 1;
                        }
                    }
                }
                break;
            }
            case sizeof(short): {
                short *v = (short *)val->value;
                if (controller[CONT_P1].keyLeft.press) {
                    if (!val->type) {
                        *v ^= 1;
                    }
                    else {
                        if (*v - 1 >= val->min) {
                            *v -= 1;
                        }
                    }
                }

                if (controller[CONT_P1].keyRight.press) {
                    if (!val->type) {
                        *v ^= 1;
                    }
                    else {
                        if (*v + 1 <= val->max) {
                            *v += 1;
                        }
                    }
                }
                break;
            }
            case sizeof(int): {
                int *v = (int *)val->value;
                if (controller[CONT_P1].keyLeft.press) {
                    if (!val->type) {
                        *v ^= 1;
                    }
                    else {
                        if (*v - 1 >= val->min) {
                            *v -= 1;
                        }
                    }
                }

                if (controller[CONT_P1].keyRight.press) {
                    if (!val->type) {
                        *v ^= 1;
                    }
                    else {
                        if (*v + 1 <= val->max) {
                            *v += 1;
                        }
                    }
                }
                break;
            }
        }
    }
    else {
        if (confirm) {
            devMenu.state  = DevMenu_Options;
            devMenu.option = 4;
        }
    }

#if !RETRO_USE_ORIGINAL_CODE
    if (controller[CONT_P1].keyB.press) {
        devMenu.state  = DevMenu_Options;
        devMenu.option = 4;
    }
#endif
}
#endif

#if RETRO_USE_MOD_LOADER
void DevMenu_Mods()
{
    int dy = currentScreen->center.y;
    DrawRectangle(currentScreen->center.x - 128, dy - 84, 256, 48, 128, 255, INK_NONE, true);
    dy -= 68;
    DrawDevText("MANAGE MODS", currentScreen->center.x, dy, ALIGN_CENTER, 0xF0F0F0);
    DrawRectangle(currentScreen->center.x - 128, dy + 36, 256, 72, 128, 255, INK_NONE, true);

    uint optionColours[8];
    optionColours[0]                               = 0x808090;
    optionColours[1]                               = 0x808090;
    optionColours[2]                               = 0x808090;
    optionColours[3]                               = 0x808090;
    optionColours[4]                               = 0x808090;
    optionColours[5]                               = 0x808090;
    optionColours[6]                               = 0x808090;
    optionColours[7]                               = 0x808090;
    optionColours[devMenu.option - devMenu.scroll] = 0xF0F0F0;

    int y               = dy + 40;
    for (int i = 0; i < 8; ++i) {
        if (devMenu.scroll + i < RSDK::modList.size()) {
            DrawDevText(RSDK::modList[(devMenu.scroll + i)].name.c_str(), currentScreen->center.x - 96, y, ALIGN_LEFT, optionColours[i]);
            DrawDevText(RSDK::modList[(devMenu.scroll + i)].active ? "Y" : "N", currentScreen->center.x + 96, y, ALIGN_RIGHT, optionColours[i]);
            y += 8;
            devMenu.scroll = devMenu.scroll;
        }
    }

#if !RETRO_USE_ORIGINAL_CODE
    DevMenu_HandleTouchControls();
#endif

    int preOption = devMenu.option;
    if (controller[CONT_P1].keyUp.press) {
        devMenu.option--;
        if (devMenu.option < 0) {
            devMenu.option = RSDK::modList.size() - 1;
        }

        if (devMenu.option >= devMenu.scroll) {
            if (devMenu.option > devMenu.scroll + 7) {
                devMenu.scroll = devMenu.option - 7;
            }
        }
        else {
            devMenu.scroll = devMenu.option;
        }
        devMenu.timer = 1;
    }
    else if (controller[CONT_P1].keyUp.down) {
        if (!devMenu.timer) {
            devMenu.option--;
            if (devMenu.option < 0) {
                devMenu.option = RSDK::modList.size() - 1;
            }
        }

        devMenu.timer = (devMenu.timer + 1) & 7;
        if (devMenu.option >= devMenu.scroll) {
            if (devMenu.option > devMenu.scroll + 7) {
                devMenu.scroll = devMenu.option - 7;
            }
        }
        else {
            devMenu.scroll = devMenu.option;
        }
    }

    if (controller[CONT_P1].keyDown.press) {
        devMenu.option++;
        if (devMenu.option >= RSDK::modList.size()) {
            devMenu.option = 0;
        }

        if (devMenu.option >= devMenu.scroll) {
            if (devMenu.option > devMenu.scroll + 7) {
                devMenu.scroll = devMenu.option - 7;
            }
        }
        else {
            devMenu.scroll = devMenu.option;
        }
        devMenu.timer = 1;
    }
    else if (controller[CONT_P1].keyDown.down) {
        if (!devMenu.timer) {
            devMenu.option++;
            if (devMenu.option >= RSDK::modList.size()) {
                devMenu.option = 0;
            }
        }

        devMenu.timer = (devMenu.timer + 1) & 7;
        if (devMenu.option >= devMenu.scroll) {
            if (devMenu.option > devMenu.scroll + 7) {
                devMenu.scroll = devMenu.option - 7;
            }
        }
        else {
            devMenu.scroll = devMenu.option;
        }
    }
    bool32 confirm = controller[CONT_P1].keyA.press;
#if RETRO_REV02
    if (RSDK::SKU::userCore->GetConfirmButtonFlip())
#else
    if (RSDK::SKU::GetConfirmButtonFlip())
#endif
        confirm = controller[CONT_P1].keyB.press;

    if (controller[CONT_P1].keyStart.press || confirm || controller[CONT_P1].keyLeft.press || controller[CONT_P1].keyRight.press) {
        RSDK::modList[devMenu.option].active ^= true;
        devMenu.modsChanged = true;
    }
    else if (controller[CONT_P1].keyC.down) {
        RSDK::ModInfo swap            = RSDK::modList[preOption];
        RSDK::modList[preOption]      = RSDK::modList[devMenu.option];
        RSDK::modList[devMenu.option] = swap;
        devMenu.modsChanged = true;
    }
    else if (controller[CONT_P1].keyB.press) {
        devMenu.state   = DevMenu_MainMenu;
        devMenu.scroll  = 0;
        devMenu.option  = 4;
        RSDK::saveMods();
        LoadGameConfig();
    }
}
#endif