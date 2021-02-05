#include "RetroEngine.hpp"

bool engineDebugMode = true;

int debugValCnt = 0;
DebugValueInfo debugValues[DEBUGVAL_MAX];

DevMenu devMenu = DevMenu();

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