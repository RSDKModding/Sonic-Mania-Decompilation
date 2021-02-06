#ifndef DEBUG_H
#define DEBUG_H

#define DEBUGVAL_MAX (0x900)

#include <stdarg.h>

enum SeverityModes {
    SEVERITY_NONE,
    SEVERITY_WARN,
    SEVERITY_ERROR,
    SEVERITY_FATAL,
};

extern bool engineDebugMode;
extern char outputString[0x400];

void printLog(SeverityModes severity, const char *message, ...);
inline void printConsole(const char *message) { printf(message); }

inline void printString(SeverityModes severity, const char *message) { printLog(severity, "%s\n", message); }
inline void printText(SeverityModes severity, const char *message, TextInfo *text)
{
    for (int c = 0; c < text->textLength; ++c) {
        printLog(severity, "%c", text->text[c]);
    }
    printLog(severity, "\n");
}
inline void printIntegerUnsigned(SeverityModes severity, const char *message, uint integer) { printLog(severity, "%s: %d", message, integer); }
inline void printInteger(SeverityModes severity, const char *message, int integer) { printLog(severity, "%s: %d", message, integer); }
inline void printFloat(SeverityModes severity, const char *message, float f) { printLog(severity, "%s: %f", message, f); }
inline void printVector2(SeverityModes severity, const char *message, int x, int y)
{
    int absX = abs(x);
    int absY = abs(y);
    int xCnt = 32;
    if (x < 0)
        xCnt = 45;
    int yCnt = 32;
    if (y < 0)
        yCnt = 45;
    printLog(severity, "%s: <%c%08X, %c%08X>", message, xCnt, absX, yCnt, absY);
}
inline void printHitbox(SeverityModes severity, const char *message, Hitbox *hitbox)
{
    printLog(severity, "%s: <l: %d, r: %d, t: %d, b: %d>", message, hitbox->left, hitbox->right, hitbox->top, hitbox->bottom);
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

inline void ClearDebugValues() { debugValCnt = 0; }
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
