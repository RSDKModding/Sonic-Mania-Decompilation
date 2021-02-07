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
                  (4 * userStorage[DATASET_STG].usedStorage) / (float)userStorage[DATASET_STG].storageLimit * 126.0, 6, 0xF0F0F0, 255, INK_NONE,
                  true);
    DrawDevText(currentScreen->centerX - 64, "STG", y, 0, 0xF0F080);

    //Music Storage
    y += 10;
    DrawRectangle(currentScreen->centerX - 40, y, 128, 8, 128, 255, INK_NONE, true);
    DrawRectangle(currentScreen->centerX - 39, y + 1,
                  (4 * userStorage[DATASET_MUS].usedStorage) / (float)userStorage[DATASET_MUS].storageLimit * 126.0, 6, 0xF0F0F0, 255, INK_NONE,
                  true);
    DrawDevText(currentScreen->centerX - 64, "MUS", y, 0, 0xF0F080);

    //SoundFX Storage
    y += 10;
    DrawRectangle(currentScreen->centerX - 40, y, 128, 8, 128, 255, INK_NONE, true);
    DrawRectangle(currentScreen->centerX - 39, y + 1,
                  (4 * userStorage[DATASET_SFX].usedStorage) / (float)userStorage[DATASET_SFX].storageLimit * 126.0, 6, 0xF0F0F0, 255, INK_NONE,
                  true);
    DrawDevText(currentScreen->centerX - 64, "SFX", y, 0, 0xF0F080);

    //String Storage
    y += 10;
    DrawRectangle(currentScreen->centerX - 40, y, 128, 8, 128, 255, INK_NONE, true);
    DrawRectangle(currentScreen->centerX - 39, y + 1,
                  (4 * userStorage[DATASET_STR].usedStorage) / (float)userStorage[DATASET_STR].storageLimit * 126.0, 6, 0xF0F0F0, 255, INK_NONE,
                  true);
    DrawDevText(currentScreen->centerX - 64, "STR", y, 0, 0xF0F080);

    //Temp Storage
    y += 10;
    DrawRectangle(currentScreen->centerX - 40, y, 128, 8, 128, 255, INK_NONE, true);
    DrawRectangle(currentScreen->centerX - 39, y + 1,
                  (4 * userStorage[DATASET_TMP].usedStorage) / (float)userStorage[DATASET_TMP].storageLimit * 126.0, 6, 0xF0F0F0, 255, INK_NONE,
                  true);
    DrawDevText(currentScreen->centerX - 64, "TMP", y, 0, 0xF0F080);

    //Temp lol
    bool Key_Up = false;
    bool Key_Down = false;
    bool Key_A = false;
    bool Key_Enter = false;
    if (Key_Up) {
        if (devMenu.timer) {
            devMenu.timer = ++devMenu.timer & 7;
            return;
        }

        devMenu.option--;
        if (devMenu.option >= 0) {
            devMenu.timer = ++devMenu.timer & 7;
            return;
        }
        devMenu.option += 5;
    }
    if (Key_Down) {
        if (devMenu.timer) {
            devMenu.timer = ++devMenu.timer & 7;
            return;
        }

        ++devMenu.option;
        if (devMenu.option > 4) {
            devMenu.timer = ++devMenu.timer & 7;
            return;
        }
    }

    devMenu.timer = 0;
    if ((Key_Enter | Key_A)) {
        switch (devMenu.option) {
            case 0:
                sceneInfo.state = devMenu.listPos;
                break;
            case 1: sceneInfo.state = ENGINESTATE_LOAD; break;
            case 2:
                devMenu.state  = DevMenu_ListSel;
                devMenu.option = 0;
                break;
            case 3:
                devMenu.state  = DevMenu_Options;
                devMenu.option = 0;
                break;
            case 4: engine.running = false; break;
            default: break;
        }
    }
}
void DevMenu_ListSel() {}
void DevMenu_SceneSel() {}
void DevMenu_Options() {}
void DevMenu_VideoOptions() {}
void DevMenu_AudioOptions() {}
void DevMenu_InputOptions() {}
void DevMenu_ButtonsOptions() {}
void DevMenu_DebugOptions() {}