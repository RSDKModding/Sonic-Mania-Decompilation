#ifndef DEBUG_H
#define DEBUG_H

#define DEBUGVAL_MAX (0x900)

#include <stdarg.h>

extern bool engineDebugMode;

inline void printLog(const char *msg, ...)
{
#ifndef RETRO_DISABLE_LOG
    if (engineDebugMode) {
        char buffer[0x100];

        // make the full string
        va_list args;
        va_start(args, msg);
        vsprintf(buffer, msg, args);
        printf("%s\n", buffer);
        sprintf(buffer, "%s\n", buffer);

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

inline void PrintConsoleDebug(const char *message, ...)
{
    char buffer[0x100];
    va_list va;

    va_start(va, message);
    vsprintf(buffer, message, va);
}

struct DebugValueInfo {
    char name[0x10];
    void *value;
    int field_14;
    int valByteCnt;
    int unknown1;
    int unknown2;
};

extern int debugValCnt;
extern DebugValueInfo debugValues[DEBUGVAL_MAX];

inline void ResetDebugValues() { debugValCnt = 0; }
void SetDebugValue(const char *name, int valPtr, int type, int unknown1, int unknown2);

struct DevMenu {
    void (*state)(void);
    int option;
    int scroll;
    int timer;
};

extern DevMenu devMenu;

void DevMenu_MainMenu();
void DevMenu_ListSel();
void DevMenu_SceneSel();
void DevMenu_Options();
void DevMenu_VideoOptions();
void DevMenu_AudioOptions();
void DevMenu_InputOptions();
void DevMenu_ButtonsOptions();
void DevMenu_DebugOptions();

#endif
