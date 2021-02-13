#include "RetroEngine.hpp"

bool engineDebugMode = true;
char outputString[0x400];

int debugValCnt = 0;
DebugValueInfo debugValues[DEBUGVAL_MAX];

DevMenu devMenu = DevMenu();

void printLog(SeverityModes severity, const char* message, ...) {
#ifndef RETRO_DISABLE_LOG
    if (engineDebugMode) {
        char buffer[0x100];

        // make the full string
        va_list args;
        va_start(args, message);
        vsprintf(buffer, message, args);
        printf("%s\n", buffer);
        sprintf(buffer, "%s\n", buffer);

        if (severity == SEVERITY_WARN) {
            if (sceneInfo.state & ENGINESTATE_FROZEN) {
                SpawnEntity(DevOutput->objectID, outputString, 0, 0);
                return;
            }
        }
        else if (severity == SEVERITY_ERROR) {
            if (sceneInfo.state & ENGINESTATE_FROZEN) {
                engine.prevEngineMode = sceneInfo.state;
                sceneInfo.state = ENGINESTATE_ERRORMSG;
                return;
            }
        }
        else if (severity == SEVERITY_FATAL && sceneInfo.state & ENGINESTATE_FROZEN) {
            engine.prevEngineMode           = sceneInfo.state;
            sceneInfo.state = ENGINESTATE_ERRORMSG_FATAL;
            return;
        }

        if (engine.printConsole)
            printConsole(outputString);
        //else
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

void SetDebugValue(const char* name, int valPtr, int type, int unknown1, int unknown2) {
    if (debugValCnt < 0x900) {
        DebugValueInfo *value = &debugValues[debugValCnt];
        strncpy(value->name, name, 0x10u);
        value->value = (int *)valPtr;
        switch (type) {
            case 1:
                value->field_14   = 0;
                value->valByteCnt = 4;
                break;
            case 2:
                value->field_14   = 1;
                value->valByteCnt = 1;
                break;
            case 3:
                value->field_14   = 1;
                value->valByteCnt = 2;
                break;
            case 4:
                value->field_14   = 1;
                value->valByteCnt = 4;
                break;
            case 6:
                value->field_14   = 2;
                value->valByteCnt = 1;
                break;
            case 7:
                value->field_14   = 2;
                value->valByteCnt = 2;
                break;
            case 8:
                value->field_14   = 2;
                value->valByteCnt = 4;
                break;
            default: break;
        }
        value->unknown1 = unknown1;
        value->unknown2 = unknown2;
        ++debugValCnt;
    }
}

void DevMenu_MainMenu()
{
    uint optionColours[5];
    optionColours[0]              = 0x808090;
    optionColours[1]              = 0x808090;
    optionColours[2]              = 0x808090;
    optionColours[3]              = 0x808090;
    optionColours[4]              = 0x808090;
    optionColours[devMenu.option] = 0xF0F0F0;

    //Info Box
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

    //Options Box
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

    //Stage Storage
    DrawRectangle(currentScreen->centerX - 40, y, 128, 8, 128, 255, INK_NONE, true);
    DrawRectangle(currentScreen->centerX - 39, y + 1,
                  (sizeof(int) * userStorage[DATASET_STG].usedStorage) / (float)userStorage[DATASET_STG].storageLimit * 126.0, 6, 0xF0F0F0, 255,
                  INK_NONE, true);
    DrawDevText(currentScreen->centerX - 64, "STG", y, 0, 0xF0F080);

    //Music Storage
    y += 10;
    DrawRectangle(currentScreen->centerX - 40, y, 128, 8, 128, 255, INK_NONE, true);
    DrawRectangle(currentScreen->centerX - 39, y + 1,
                  (sizeof(int) * userStorage[DATASET_MUS].usedStorage) / (float)userStorage[DATASET_MUS].storageLimit * 126.0, 6, 0xF0F0F0, 255,
                  INK_NONE, true);
    DrawDevText(currentScreen->centerX - 64, "MUS", y, 0, 0xF0F080);

    //SoundFX Storage
    y += 10;
    DrawRectangle(currentScreen->centerX - 40, y, 128, 8, 128, 255, INK_NONE, true);
    DrawRectangle(currentScreen->centerX - 39, y + 1,
                  (sizeof(int) * userStorage[DATASET_SFX].usedStorage) / (float)userStorage[DATASET_SFX].storageLimit * 126.0, 6, 0xF0F0F0, 255,
                  INK_NONE, true);
    DrawDevText(currentScreen->centerX - 64, "SFX", y, 0, 0xF0F080);

    //String Storage
    y += 10;
    DrawRectangle(currentScreen->centerX - 40, y, 128, 8, 128, 255, INK_NONE, true);
    DrawRectangle(currentScreen->centerX - 39, y + 1,
                  (sizeof(int) * userStorage[DATASET_STR].usedStorage) / (float)userStorage[DATASET_STR].storageLimit * 126.0, 6, 0xF0F0F0, 255,
                  INK_NONE, true);
    DrawDevText(currentScreen->centerX - 64, "STR", y, 0, 0xF0F080);

    //Temp Storage
    y += 10;
    DrawRectangle(currentScreen->centerX - 40, y, 128, 8, 128, 255, INK_NONE, true);
    DrawRectangle(currentScreen->centerX - 39, y + 1,
                  (sizeof(int) * userStorage[DATASET_TMP].usedStorage) / (float)userStorage[DATASET_TMP].storageLimit * 126.0, 6, 0xF0F0F0, 255,
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
            case 0:
                sceneInfo.state = devMenu.stateStore;
                break;
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
    else 
    if (controller[0].keyUp.down) {
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
            devMenu.state    = DevMenu_SceneSel;
            devMenu.listPos  = devMenu.option;
            devMenu.scroll   = 0;
            devMenu.option   = 0;
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
    int off = list->sceneOffsetStart;
    for (int i = 0; i < 8; ++i) {
        if (devMenu.scroll + i < list->sceneCount) {
            DrawDevText(currentScreen->centerX + 96, sceneInfo.listData[off + (devMenu.scroll + i)].name, y, ALIGN_RIGHT, optionColours[i]);
            y += 8;
            devMenu.scroll = devMenu.scroll;
        }
    }

    if (controller[0].keyUp.press) {
        if (!devMenu.timer) {
            devMenu.option--;
            if (off + devMenu.option < list->sceneOffsetStart) {
                devMenu.option = list->sceneCount - 1;
            }
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
        sceneInfo.listPos        = devMenu.option + sceneInfo.listCategory[devMenu.listPos].sceneOffsetStart;
        sceneInfo.state          = ENGINESTATE_LOAD;
    }
}
void DevMenu_Options() { currentScreen = &screens[0]; }
void DevMenu_VideoOptions() { currentScreen = &screens[0]; }
void DevMenu_AudioOptions() { currentScreen = &screens[0]; }
void DevMenu_InputOptions() { currentScreen = &screens[0]; }
void DevMenu_ButtonsOptions() { currentScreen = &screens[0]; }
void DevMenu_DebugOptions() { currentScreen = &screens[0]; }