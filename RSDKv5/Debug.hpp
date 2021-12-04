#ifndef DEBUG_H
#define DEBUG_H

#define DEBUGVAL_MAX (0x900)

#include <stdarg.h>

extern bool32 engineDebugMode;
extern bool32 useEndLine;
extern char outputString[0x400];

void PrintLog(SeverityModes severity, const char *message, ...);
inline void printConsole(const char *message) { printf("%s", message); }

#if RETRO_REV02
inline void PrintString(SeverityModes severity, const char *message) { PrintLog(severity, "%s", message); }
inline void PrintText(SeverityModes severity, TextInfo *text)
{
    useEndLine = false;
    for (int c = 0; c < text->length; ++c) {
        PrintLog(severity, "%c", text->text[c]);
    }
    PrintLog(severity, "\n");
    useEndLine = true;
}
inline void PrintIntegerUnsigned(SeverityModes severity, const char *message, uint integer) { PrintLog(severity, "%s: %d", message, integer); }
inline void PrintInteger(SeverityModes severity, const char *message, int32 integer) { PrintLog(severity, "%s: %d", message, integer); }
inline void PrintFloat(SeverityModes severity, const char *message, float f) { PrintLog(severity, "%s: %f", message, f); }
inline void PrintVector2(SeverityModes severity, const char *message, int32 x, int32 y)
{
    int32 absX = abs(x);
    int32 absY = abs(y);
    int32 xCnt = 32;
    if (x < 0)
        xCnt = 45;
    int32 yCnt = 32;
    if (y < 0)
        yCnt = 45;
    PrintLog(severity, "%s: <%c%08X, %c%08X>", message, xCnt, absX, yCnt, absY);
}
inline void PrintHitbox(SeverityModes severity, const char *message, Hitbox *hitbox)
{
    PrintLog(severity, "%s: <l: %d, r: %d, t: %d, b: %d>", message, hitbox->left, hitbox->right, hitbox->top, hitbox->bottom);
}

struct DebugValueInfo {
    char name[0x10];
    void *value;
    int32 type;
    int32 valByteCnt;
    int32 min;
    int32 max;
};

typedef enum {
    DTYPE_BOOL = 1,
    DTYPE_UINT8,
    DTYPE_UINT16,
    DTYPE_UINT32,
    DTYPE_INT8,
    DTYPE_INT16,
    DTYPE_INT32,
} DebugVarTypes;

extern int32 debugValCnt;
extern DebugValueInfo debugValues[DEBUGVAL_MAX];

inline void ClearDebugValues() { debugValCnt = 0; }
void SetDebugValue(const char *name, void *valPtr, int32 type, int32 min, int32 max);
#endif

#if !RETRO_REV02
void PrintMessage(void *msg, int32 type);
#endif

struct DevMenu {
    void (*state)(void);
    int32 option;
    int32 scroll;
    int32 timer;
    int32 listPos;
    int32 stateStore;
    int32 winScale;
    int32 winAspect;
    bool32 windowed;
#if RETRO_USE_MOD_LOADER
    bool32 modsChanged;
#endif
};

extern DevMenu devMenu;

void DevMenu_MainMenu();
void DevMenu_ListSel();
void DevMenu_SceneSel();
void DevMenu_Options();
void DevMenu_VideoOptions();
void DevMenu_AudioOptions();
void DevMenu_InputOptions();
void DevMenu_MappingsOptions();
#if RETRO_REV02
void DevMenu_DebugOptions();
#endif 
#if RETRO_USE_MOD_LOADER
void DevMenu_Mods();
#endif

#endif
