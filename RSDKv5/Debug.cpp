#include "RetroEngine.hpp"

bool32 engineDebugMode = true;
char outputString[0x400];

#if RETRO_REV02
int debugValCnt = 0;
DebugValueInfo debugValues[DEBUGVAL_MAX];
#endif

DevMenu devMenu = DevMenu();

void printLog(SeverityModes severity, const char *message, ...)
{
#ifndef RETRO_DISABLE_LOG
    if (engineDebugMode) {
        char buffer[0x100];

        // make the full string
        va_list args;
        va_start(args, message);
        vsprintf(buffer, message, args);
        printf("%s\n", buffer);
        sprintf(buffer, "%s\n", buffer);
        va_end(args);

        if (severity == SEVERITY_WARN) {
            if (sceneInfo.state & ENGINESTATE_FROZEN) {
                CreateEntity(DevOutput->objectID, outputString, 0, 0);
                return;
            }
        }
        else if (severity == SEVERITY_ERROR) {
            if (sceneInfo.state & ENGINESTATE_FROZEN) {
                engine.prevEngineMode = sceneInfo.state;
                sceneInfo.state       = ENGINESTATE_ERRORMSG;
                return;
            }
        }
        else if (severity == SEVERITY_FATAL && sceneInfo.state & ENGINESTATE_FROZEN) {
            engine.prevEngineMode = sceneInfo.state;
            sceneInfo.state       = ENGINESTATE_ERRORMSG_FATAL;
            return;
        }

        if (engine.printConsole)
            printConsole(outputString);
        // else
        //    OutputDebugStringA(outputString);

        char pathBuffer[0x100];
        sprintf(pathBuffer, BASE_PATH "log.txt");
        FileIO *file = fOpen(pathBuffer, "a");
        if (file) {
            fWrite(&buffer, 1, StrLength(buffer), file);
            fClose(file);
        }
    }
#endif
}

#if RETRO_REV02
void SetDebugValue(const char *name, void* valPtr, int type, int min, int max)
{
    if (debugValCnt < DEBUGVAL_MAX) {
        DebugValueInfo *value = &debugValues[debugValCnt++];
        strncpy(value->name, name, 0x10);
        value->value = valPtr;
        switch (type) {
            case 1: //bool
                value->type   = 0;
                value->valByteCnt = 4;
                break;
            case 2: //byte
                value->type   = 1;
                value->valByteCnt = 1;
                break;
            case 3: //ushort
                value->type   = 1;
                value->valByteCnt = 2;
                break;
            case 4: //uint
                value->type   = 1;
                value->valByteCnt = 4;
                break;
            case 6: //sbyte
                value->type   = 2;
                value->valByteCnt = 1;
                break;
            case 7: //short
                value->type   = 2;
                value->valByteCnt = 2;
                break;
            case 8: //int
                value->type   = 2;
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
            case 0: printLog(SEVERITY_NONE, "%s", (const char *)msg); break;
            case 1: printLog(SEVERITY_NONE, "%i", *(signed int *)msg); break;
            case 2: printLog(SEVERITY_NONE, "%i", *(int *)msg, 0); break;
            case 3: printLog(SEVERITY_NONE, "%f", *(float *)msg); break;
            default: break;
        }
    }
}
#endif

void DevMenu_MainMenu()
{
    uint optionColours[5];
    optionColours[0]              = 0x808090;
    optionColours[1]              = 0x808090;
    optionColours[2]              = 0x808090;
    optionColours[3]              = 0x808090;
    optionColours[4]              = 0x808090;
    optionColours[devMenu.option] = 0xF0F0F0;

    // Info Box
    int y = currentScreen->centerY - 80;
    DrawRectangle(currentScreen->centerX - 128, currentScreen->centerY - 84, 256, 48, 128, 255, INK_NONE, true);
    DrawDevText(currentScreen->centerX, "RETRO ENGINE v5", y, ALIGN_CENTER, 0xF0F0F0);
    y += 8;
    DrawDevText(currentScreen->centerX, "Dev Menu", y, ALIGN_CENTER, 0xF0F0F0);
    y += 16;
    DrawDevText(currentScreen->centerX, engine.gameName, y, ALIGN_CENTER, 0x808090);
    y += 8;
    DrawDevText(currentScreen->centerX, engine.gameVersion, y, ALIGN_CENTER, 0x808090);
    y += 24;

    // Options Box
    DrawRectangle(currentScreen->centerX - 128, y - 8, 256, 72, 128, 255, INK_NONE, true);
    DrawDevText(currentScreen->centerX, "Resume", y, ALIGN_CENTER, optionColours[0]);
    y += 12;
    DrawDevText(currentScreen->centerX, "Restart", y, ALIGN_CENTER, optionColours[1]);
    y += 12;
    DrawDevText(currentScreen->centerX, "Stage Select", y, ALIGN_CENTER, optionColours[2]);
    y += 12;
    DrawDevText(currentScreen->centerX, "Options", y, ALIGN_CENTER, optionColours[3]);
    y += 12;
    DrawDevText(currentScreen->centerX, "Exit", y, ALIGN_CENTER, optionColours[4]);
    y += 32;

    // Storage box

    // Stage Storage
    DrawRectangle(currentScreen->centerX - 40, y, 128, 8, 128, 255, INK_NONE, true);
    DrawRectangle(currentScreen->centerX - 39, y + 1,
                  (sizeof(int) * dataStorage[DATASET_STG].usedStorage) / (float)dataStorage[DATASET_STG].storageLimit * 126.0, 6, 0xF0F0F0, 255,
                  INK_NONE, true);
    DrawDevText(currentScreen->centerX - 64, "STG", y, 0, 0xF0F080);

    // Music Storage
    y += 10;
    DrawRectangle(currentScreen->centerX - 40, y, 128, 8, 128, 255, INK_NONE, true);
    DrawRectangle(currentScreen->centerX - 39, y + 1,
                  (sizeof(int) * dataStorage[DATASET_MUS].usedStorage) / (float)dataStorage[DATASET_MUS].storageLimit * 126.0, 6, 0xF0F0F0, 255,
                  INK_NONE, true);
    DrawDevText(currentScreen->centerX - 64, "MUS", y, 0, 0xF0F080);

    // SoundFX Storage
    y += 10;
    DrawRectangle(currentScreen->centerX - 40, y, 128, 8, 128, 255, INK_NONE, true);
    DrawRectangle(currentScreen->centerX - 39, y + 1,
                  (sizeof(int) * dataStorage[DATASET_SFX].usedStorage) / (float)dataStorage[DATASET_SFX].storageLimit * 126.0, 6, 0xF0F0F0, 255,
                  INK_NONE, true);
    DrawDevText(currentScreen->centerX - 64, "SFX", y, 0, 0xF0F080);

    // String Storage
    y += 10;
    DrawRectangle(currentScreen->centerX - 40, y, 128, 8, 128, 255, INK_NONE, true);
    DrawRectangle(currentScreen->centerX - 39, y + 1,
                  (sizeof(int) * dataStorage[DATASET_STR].usedStorage) / (float)dataStorage[DATASET_STR].storageLimit * 126.0, 6, 0xF0F0F0, 255,
                  INK_NONE, true);
    DrawDevText(currentScreen->centerX - 64, "STR", y, 0, 0xF0F080);

    // Temp Storage
    y += 10;
    DrawRectangle(currentScreen->centerX - 40, y, 128, 8, 128, 255, INK_NONE, true);
    DrawRectangle(currentScreen->centerX - 39, y + 1,
                  (sizeof(int) * dataStorage[DATASET_TMP].usedStorage) / (float)dataStorage[DATASET_TMP].storageLimit * 126.0, 6, 0xF0F0F0, 255,
                  INK_NONE, true);
    DrawDevText(currentScreen->centerX - 64, "TMP", y, 0, 0xF0F080);

    if (controller[0].keyUp.press) {
        devMenu.option--;
        devMenu.timer = 1;
        if (devMenu.option < 0)
            devMenu.option += 5;
    }
    else if (controller[0].keyUp.down) {
        if (devMenu.timer) {
            devMenu.timer = ++devMenu.timer & 7;
        }
        else {
            devMenu.option--;
            devMenu.timer = ++devMenu.timer & 7;
            if (devMenu.option < 0)
                devMenu.option += 5;
        }
    }

    if (controller[0].keyDown.press) {
        devMenu.option++;
        devMenu.timer = 1;
        if (devMenu.option > 4)
            devMenu.option -= 5;
    }
    else if (controller[0].keyDown.down) {
        if (devMenu.timer) {
            devMenu.timer = ++devMenu.timer & 7;
        }
        else {
            devMenu.option++;
            devMenu.timer = ++devMenu.timer & 7;
            if (devMenu.option > 4)
                devMenu.option -= 5;
        }
    }

    if (controller[0].keyStart.press || controller[0].keyA.press) {
        switch (devMenu.option) {
            case 0: sceneInfo.state = devMenu.stateStore; break;
            case 1: sceneInfo.state = ENGINESTATE_LOAD; break;
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
            case 4: engine.running = false; break;
            default: break;
        }
    }
}
void DevMenu_ListSel()
{
    int dy = currentScreen->centerY;
    DrawRectangle(currentScreen->centerX - 128, dy - 84, 256, 48, 128, 255, INK_NONE, true);
    dy -= 68;
    DrawDevText(currentScreen->centerX, "SELECT STAGE CATEGORY", dy, ALIGN_CENTER, 0xF0F0F0);
    DrawRectangle(currentScreen->centerX - 128, dy + 36, 256, 72, 128, 255, INK_NONE, true);

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
            DrawDevText(currentScreen->centerX - 64, sceneInfo.listCategory[devMenu.scroll + i].name, y, 0, optionColours[i]);
            y += 8;
        }
    }

    if (controller[0].keyUp.press) {
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
    else if (controller[0].keyUp.down) {
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

    if (controller[0].keyDown.press) {
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
    else if (controller[0].keyDown.down) {
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

    if (controller[0].keyStart.press || controller[0].keyA.press) {
        if (sceneInfo.listCategory[devMenu.option].sceneCount) {
            devMenu.state   = DevMenu_SceneSel;
            devMenu.listPos = devMenu.option;
            devMenu.scroll  = 0;
            devMenu.option  = 0;
        }
    }
}
void DevMenu_SceneSel()
{
    int dy = currentScreen->centerY;
    DrawRectangle(currentScreen->centerX - 128, dy - 84, 256, 48, 128, 255, INK_NONE, true);
    dy -= 68;
    DrawDevText(currentScreen->centerX, "SELECT STAGE SCENE", dy, ALIGN_CENTER, 0xF0F0F0);
    DrawRectangle(currentScreen->centerX - 128, dy + 36, 256, 72, 128, 255, INK_NONE, true);

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
            DrawDevText(currentScreen->centerX + 96, sceneInfo.listData[off + (devMenu.scroll + i)].name, y, ALIGN_RIGHT, optionColours[i]);
            y += 8;
            devMenu.scroll = devMenu.scroll;
        }
    }

    if (controller[0].keyUp.press) {
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
    else if (controller[0].keyUp.down) {
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

    if (controller[0].keyDown.press) {
        devMenu.option++;
        if (devMenu.option > list->sceneCount) {
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
    else if (controller[0].keyDown.down) {
        if (!devMenu.timer) {
            devMenu.option++;
            if (devMenu.option > list->sceneCount) {
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

    if (controller[0].keyStart.press || controller[0].keyA.press) {
        sceneInfo.activeCategory = devMenu.listPos;
        sceneInfo.listPos        = devMenu.option;
        sceneInfo.state          = ENGINESTATE_LOAD;
    }
}
void DevMenu_Options()
{
    int dy = currentScreen->centerY;
    DrawRectangle(currentScreen->centerX - 128, dy - 84, 256, 48, 128, 255, INK_NONE, true);
    dy -= 68;
    DrawDevText(currentScreen->centerX, "OPTIONS", dy, ALIGN_CENTER, 0xF0F0F0);
    dy += 44;
    DrawRectangle(currentScreen->centerX - 128, dy - 8, 256, 72, 128, 255, INK_NONE, true);

    const byte optionCount = RETRO_REV02 ? 5 : 4;

    uint optionColours[optionCount];
    optionColours[0]              = 0x808090;
    optionColours[1]              = 0x808090;
    optionColours[2]              = 0x808090;
    optionColours[3]              = 0x808090;
#if RETRO_REV02
    optionColours[4]              = 0x808090;
#endif
    optionColours[devMenu.option] = 0xF0F0F0;

    DrawDevText(currentScreen->centerX, "Video Settings", dy, ALIGN_CENTER, optionColours[0]);
    dy += 12;
    DrawDevText(currentScreen->centerX, "Audio Settings", dy, ALIGN_CENTER, optionColours[1]);
    dy += 12;
    DrawDevText(currentScreen->centerX, "Configure Input", dy, ALIGN_CENTER, optionColours[2]);
#if RETRO_REV02
    dy += 12;
    DrawDevText(currentScreen->centerX, "Debug Flags", dy, ALIGN_CENTER, optionColours[3]);
#endif
    DrawDevText(currentScreen->centerX, "Back", dy + 12, ALIGN_CENTER, optionColours[optionCount - 1]);

    if (controller[0].keyUp.press) {
        devMenu.option--;
        if (devMenu.option < 0) {
            devMenu.option = optionCount - 1;
        }

        devMenu.timer = 1;
    }
    else if (controller[0].keyUp.down) {
        if (!devMenu.timer) {
            devMenu.option--;
            if (devMenu.option < 0) {
                devMenu.option = optionCount - 1;
            }
        }

        devMenu.timer = (devMenu.timer + 1) & 7;
    }

    if (controller[0].keyDown.press) {
        devMenu.option++;
        if (devMenu.option >= optionCount) {
            devMenu.option = 0;
        }

        devMenu.timer = 1;
    }
    else if (controller[0].keyDown.down) {
        if (!devMenu.timer) {
            devMenu.option++;
            if (devMenu.option >= optionCount) {
                devMenu.option = 0;
            }
        }

        devMenu.timer = (devMenu.timer + 1) & 7;
    }

    if (controller[0].keyStart.press || controller[0].keyA.press) {
        switch (devMenu.option) {
            case 0: {
                devMenu.windowed = !engine.isFullScreen;
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
}
void DevMenu_VideoOptions()
{
    int dy = currentScreen->centerY;
    DrawRectangle(currentScreen->centerX - 128, dy - 84, 256, 48, 128, 255, INK_NONE, true);
    dy -= 68;
    DrawDevText(currentScreen->centerX, "VIDEO SETTINGS", dy, ALIGN_CENTER, 0xF0F0F0);
    dy += 44;
    DrawRectangle(currentScreen->centerX - 128, dy - 8, 256, 72, 128, 255, INK_NONE, true);

    uint optionColours[6];
    optionColours[0]              = 0x808090;
    optionColours[1]              = 0x808090;
    optionColours[2]              = 0x808090;
    optionColours[3]              = 0x808090;
    optionColours[4]              = 0x808090;
    optionColours[5]              = 0x808090;
    optionColours[devMenu.option] = 0xF0F0F0;

    DrawDevText(currentScreen->centerX - 96, "Window Size:", dy, 0, optionColours[0]);

    const char *winScale = "unknown";
    switch (devMenu.winScale) {
        case 0: winScale = "1x"; break;
        case 1: winScale = "2x"; break;
        case 2: winScale = "3x"; break;
        case 3: winScale = "4x"; break;
        default: break;
    }
    DrawDevText(currentScreen->centerX + 80, winScale, dy, ALIGN_CENTER, 0xF0F080);
    dy += 8;
    DrawDevText(currentScreen->centerX - 96, "Window Aspect:", dy, 0, optionColours[1]);

    const char *winAspect = "unknown";
    switch (devMenu.winAspect) {
        case 0: winAspect = "4:3"; break;
        case 1: winAspect = "3:2"; break;
        case 2: winAspect = "16:10"; break;
        case 3: winAspect = "5:3"; break;
        case 4: winAspect = "16:9"; break;
        default: break;
    }
    DrawDevText(currentScreen->centerX + 80, winAspect, dy, ALIGN_CENTER, 0xF0F080);
    dy += 8;
    DrawDevText(currentScreen->centerX - 96, "Fullscreen:", dy, 0, optionColours[2]);
    const char *fsOpt = "NO";
    if (devMenu.windowed != 1)
        fsOpt = "YES";
    DrawDevText(currentScreen->centerX + 80, fsOpt, dy, ALIGN_CENTER, 0xF0F080);
    dy += 8;
    DrawDevText(currentScreen->centerX - 96, "Screen Shader:", dy, ALIGN_LEFT, optionColours[3]);
    DrawDevText(currentScreen->centerX + 80, shaderList[engine.shaderID].name, dy, ALIGN_CENTER, 0xF0F080);
    dy += 16;
    DrawDevText(currentScreen->centerX, "Confirm", dy, ALIGN_CENTER, optionColours[4]);
    DrawDevText(currentScreen->centerX, "Cancel", dy + 8, ALIGN_CENTER, optionColours[5]);

    if (controller[0].keyUp.press) {
        devMenu.option--;
        if (devMenu.option < 0) {
            devMenu.option = 5;
        }

        devMenu.timer = 1;
    }
    else if (controller[0].keyUp.down) {
        if (!devMenu.timer) {
            devMenu.option--;
            if (devMenu.option < 0) {
                devMenu.option = 5;
            }
        }

        devMenu.timer = (devMenu.timer + 1) & 7;
    }

    if (controller[0].keyDown.press) {
        devMenu.option++;
        if (devMenu.option > 5) {
            devMenu.option = 0;
        }

        devMenu.timer = 1;
    }
    else if (controller[0].keyDown.down) {
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
            if (controller[0].keyLeft.press) {
                devMenu.winScale = (devMenu.winScale - 1) & 3;
                settingsChanged  = true;
            }
            else if (controller[0].keyRight.press) {
                devMenu.winScale = (devMenu.winScale + 1) & 3;
                settingsChanged  = true;
            }
            break;
        case 1: // aspect
            if (controller[0].keyLeft.press) {
                devMenu.winAspect--;
                settingsChanged  = true;
            }
            else if (controller[0].keyRight.press) {
                devMenu.winAspect++;
                settingsChanged  = true;
            }

            if (devMenu.winAspect > 4) {
                devMenu.winAspect = 0;
            }
            else if (devMenu.winAspect < 0) {
                devMenu.winAspect = 4;
            }
            break;
        case 2: // fullscreen
            if (controller[0].keyLeft.press || controller[0].keyRight.press) {
                devMenu.windowed ^= 1;
                settingsChanged = true;
            }
            break;
        case 3: // screenShader
            if (controller[0].keyLeft.press) {
                engine.shaderID--;
                settingsChanged = true;
            }
            else if (controller[0].keyRight.press) {
                engine.shaderID++;
                settingsChanged = true;
            }

            if (engine.shaderID > SHADER_CRT_YEE64) {
                engine.shaderID = SHADER_NONE;
            }
            else if (engine.shaderID < SHADER_NONE) {
                engine.shaderID = SHADER_CRT_YEE64;
            }
            break;
        case 4: // confirm
            if (controller[0].keyStart.press || controller[0].keyA.press) {
                // do confirm
                engine.isFullScreen   = !devMenu.windowed;
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
        case 5: // cancel
            if (controller[0].keyStart.press || controller[0].keyA.press) {
                devMenu.state  = DevMenu_Options;
                devMenu.option = 0;
            }
            break;
    }
}
void DevMenu_AudioOptions()
{
    int dy = currentScreen->centerY;
    DrawRectangle(currentScreen->centerX - 128, dy - 84, 256, 48, 128, 255, INK_NONE, true);
    dy -= 68;
    DrawDevText(currentScreen->centerX, "AUDIO SETTINGS", dy, ALIGN_CENTER, 0xF0F0F0);
    dy += 44;
    DrawRectangle(currentScreen->centerX - 128, dy - 8, 256, 72, 128, 255, INK_NONE, true);

    uint optionColours[4];
    optionColours[0]              = 0x808090;
    optionColours[1]              = 0x808090;
    optionColours[2]              = 0x808090;
    optionColours[3]              = 0x808090;
    optionColours[devMenu.option] = 0xF0F0F0;

    DrawDevText(currentScreen->centerX - 96, "Streams Enabled:", dy, 0, optionColours[0]);

    const char *strmEnabled = "YES";
    if (!engine.streamsEnabled)
        strmEnabled = "NO";
    DrawDevText(currentScreen->centerX + 80, strmEnabled, dy, ALIGN_CENTER, 15790208);
    dy += 16;

    DrawDevText(currentScreen->centerX - 96, "Streams Vol:", dy, 0, optionColours[1]);
    DrawRectangle(currentScreen->centerX + 8, dy, 112, 8, 0, 255, INK_NONE, true);
    DrawRectangle(currentScreen->centerX + 9, dy + 1, (int)(engine.streamVolume * 110.0), 6, 0xF0F0F0, 255, INK_NONE, true);
    dy += 16;

    DrawDevText(currentScreen->centerX - 96, "SoundFX Vol:", dy, 0, optionColours[2]);
    DrawRectangle(currentScreen->centerX + 8, dy, 112, 8, 0, 255, INK_NONE, true);
    DrawRectangle(currentScreen->centerX + 9, dy + 1, (int)(engine.soundFXVolume * 110.0), 6, 0xF0F0F0, 255, INK_NONE, true);
    DrawDevText(currentScreen->centerX, "Back", dy + 16, ALIGN_CENTER, optionColours[3]);

    if (controller[0].keyUp.press) {
        devMenu.option--;
        if (devMenu.option < 0) {
            devMenu.option = 3;
        }

        devMenu.timer = 1;
    }
    else if (controller[0].keyUp.down) {
        if (!devMenu.timer) {
            devMenu.option--;
            if (devMenu.option < 0) {
                devMenu.option = 3;
            }
        }

        devMenu.timer = (devMenu.timer + 1) & 7;
    }

    if (controller[0].keyDown.press) {
        devMenu.option++;
        if (devMenu.option > 3) {
            devMenu.option = 0;
        }

        devMenu.timer = 1;
    }
    else if (controller[0].keyDown.down) {
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
            if (controller[0].keyLeft.press || controller[0].keyRight.press) {
                engine.streamsEnabled ^= 1;
                settingsChanged = 1;
            }
            break;
        case 1:
            if (controller[0].keyLeft.down) {
                engine.streamVolume -= 0.015625;
                if (engine.streamVolume < 0.0)
                    engine.streamVolume = 0.0;
                settingsChanged = 1;
            }
            else {
                if (controller[0].keyRight.down) {
                    engine.streamVolume += 0.015625;
                    if (engine.streamVolume > 1.0)
                        engine.streamVolume = 1.0;
                    settingsChanged = 1;
                }
            }
            break;
        case 2:
            if (controller[0].keyLeft.down) {
                engine.soundFXVolume -= 0.015625;
                if (engine.soundFXVolume < 0.0)
                    engine.soundFXVolume = 0.0;
                settingsChanged = 1;
            }
            else {
                if (controller[0].keyRight.down) {
                    engine.soundFXVolume += 0.015625;
                    if (engine.soundFXVolume > 1.0)
                        engine.soundFXVolume = 1.0;
                    settingsChanged = 1;
                }
            }
            break;
        case 3:
            if (controller[0].keyStart.press || controller[0].keyA.press) {
                devMenu.state  = DevMenu_Options;
                devMenu.option = 1;
            }
            break;
    }
}
void DevMenu_InputOptions()
{
    int dy = currentScreen->centerY;
    DrawRectangle(currentScreen->centerX - 128, dy - 84, 256, 48, 128, 255, INK_NONE, true);
    dy -= 68;
    DrawDevText(currentScreen->centerX, "CONFIGURE INPUT", dy, ALIGN_CENTER, 0xF0F0F0);
    dy += 44;
    DrawRectangle(currentScreen->centerX - 128, dy - 8, 256, 72, 128, 255, INK_NONE, true);

    uint optionColours[6];
    optionColours[0]              = 0x808090;
    optionColours[1]              = 0x808090;
    optionColours[2]              = 0x808090;
    optionColours[3]              = 0x808090;
    optionColours[4]              = 0x808090;
    optionColours[devMenu.option] = 0xF0F0F0;

    DrawDevText(currentScreen->centerX, "Set Keys For Input 1", dy, ALIGN_CENTER, optionColours[0]);
    dy += 10;
    DrawDevText(currentScreen->centerX, "Set Keys For Input 2", dy, ALIGN_CENTER, optionColours[1]);
    dy += 10;
    DrawDevText(currentScreen->centerX, "Set Keys For Input 3", dy, ALIGN_CENTER, optionColours[2]);
    dy += 10;
    DrawDevText(currentScreen->centerX, "Set Keys For Input 4", dy, ALIGN_CENTER, optionColours[3]);
    DrawDevText(currentScreen->centerX, "Back", dy + 18, ALIGN_CENTER, optionColours[4]);

    if (controller[0].keyUp.press) {
        devMenu.option--;
        if (devMenu.option < 0) {
            devMenu.option = 4;
        }

        devMenu.timer = 1;
    }
    else if (controller[0].keyUp.down) {
        if (!devMenu.timer) {
            devMenu.option--;
            if (devMenu.option < 0) {
                devMenu.option = 4;
            }
        }

        devMenu.timer = (devMenu.timer + 1) & 7;
    }

    if (controller[0].keyDown.press) {
        devMenu.option++;
        if (devMenu.option > 4) {
            devMenu.option = 0;
        }

        devMenu.timer = 1;
    }
    else if (controller[0].keyDown.down) {
        if (!devMenu.timer) {
            devMenu.option++;
            if (devMenu.option > 4) {
                devMenu.option = 0;
            }
        }

        devMenu.timer = (devMenu.timer + 1) & 7;
    }

    if (controller[0].keyStart.press || controller[0].keyA.press) {
        if (devMenu.option == 4) {
            devMenu.state  = DevMenu_Options;
            devMenu.option = 3;
        }
        else {
            devMenu.state   = DevMenu_MappingsOptions;
            devMenu.scroll  = 0;
            settingsChanged = true;
        }
    }
}
void DevMenu_MappingsOptions() {}
#if RETRO_REV02
void DevMenu_DebugOptions()
{
    int dy = currentScreen->centerY;
    DrawRectangle(currentScreen->centerX - 128, dy - 84, 256, 48, 128, 255, INK_NONE, true);
    dy -= 68;
    DrawDevText(currentScreen->centerX, "CONFIGURE DEBUG FLAGS", dy, ALIGN_CENTER, 0xF0F0F0);
    dy += 40;
    DrawRectangle(currentScreen->centerX - 128, dy - 4, 256, 72, 128, 255, INK_NONE, true);
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

    bool32 confirm = controller[0].keyStart.press || controller[0].keyA.press;

    for (int i = 0; i < 8; ++i) {
        if (devMenu.scroll + i < debugValCnt) {
            DebugValueInfo *val = &debugValues[devMenu.scroll + i];
            DrawDevText(currentScreen->centerX - 96, val->name, dy, ALIGN_LEFT, optionColours[i]);
            if (!val->value) {
                DrawDevText(currentScreen->centerX + 96, "--------", dy, ALIGN_RIGHT, 0xF0F080);
            }
            else {
                char valBuf[0x10];
                StrCopy(valBuf, "--------");
                switch (val->valByteCnt) {
                    default: DrawDevText(currentScreen->centerX + 96, "--------", dy, ALIGN_RIGHT, 0xF0F080); break;
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
                        break;
                    }
                }

                if (val->type) {
                    if (2 * val->valByteCnt) {
                        char *bufPtr = &valBuf[2 * val->valByteCnt];
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
                DrawDevText(currentScreen->centerX + 96, valBuf, dy, ALIGN_CENTER, optionColours[i]);
            }
            dy += 8;
        }
        else {
            DrawDevText(currentScreen->centerX, "Back", dy, ALIGN_CENTER, optionColours[i]);
        }
    }

    if (controller[0].keyUp.press) {
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
    else if (controller[0].keyUp.down) {
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

    if (controller[0].keyDown.press) {
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
    else if (controller[0].keyDown.down) {
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
            default: DrawDevText(currentScreen->centerX + 96, "--------", dy, ALIGN_RIGHT, 0xF0F080); break;
            case sizeof(sbyte): {
                sbyte *v = (sbyte *)val->value;
                if (controller[0].keyLeft.press) {
                    if (!val->type) {
                        *v ^= 1;
                    }
                    else {
                        if (*v - 1 >= val->min) {
                            *v -= 1;
                        }
                    }
                }

                if (controller[0].keyRight.press) {
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
                if (controller[0].keyLeft.press) {
                    if (!val->type) {
                        *v ^= 1;
                    }
                    else {
                        if (*v - 1 >= val->min) {
                            *v -= 1;
                        }
                    }
                }

                if (controller[0].keyRight.press) {
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
                if (controller[0].keyLeft.press) {
                    if (!val->type) {
                        *v ^= 1;
                    }
                    else {
                        if (*v - 1 >= val->min) {
                            *v -= 1;
                        }
                    }
                }

                if (controller[0].keyRight.press) {
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

}
#endif
