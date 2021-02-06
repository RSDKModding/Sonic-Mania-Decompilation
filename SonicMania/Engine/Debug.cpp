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
            if (engine.engineMode & ENGINESTATE_FROZEN) {
                //SpawnEntity(DevOutput.objectID, outputString, 0, 0);
                return;
            }
        }
        else if (severity == SEVERITY_ERROR) {
            if (engine.engineMode & ENGINESTATE_FROZEN) {
                engine.prevEngineMode = engine.engineMode;
                engine.engineMode = ENGINESTATE_ERRORMSG;
                return;
            }
        }
        else if (severity == SEVERITY_FATAL && engine.engineMode & ENGINESTATE_FROZEN) {
            engine.prevEngineMode           = engine.engineMode;
            engine.engineMode = ENGINESTATE_ERRORMSG_FATAL;
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


void DevMenu_MainMenu() {}
void DevMenu_ListSel() {}
void DevMenu_SceneSel() {}
void DevMenu_Options() {}
void DevMenu_VideoOptions() {}
void DevMenu_AudioOptions() {}
void DevMenu_InputOptions() {}
void DevMenu_ButtonsOptions() {}
void DevMenu_DebugOptions() {}