#ifndef DEBUG_H
#define DEBUG_H

// These being different sizes will surely cause issues if more then DEBUGVALUE_LIST_COUNT values are set
// But that's how the code is in the original so what can ya do
#define DEBUGVALUE_COUNT      (0x900)
#define DEBUGVALUE_LIST_COUNT (0x40)

#include <stdarg.h>

extern bool32 engineDebugMode;
extern bool32 useEndLine;
extern char outputString[0x400];

void PrintLog(int32 severity, const char *message, ...);
inline void PrintConsole(const char *message) { printf("%s", message); }

#if !RETRO_REV02
void PrintMessage(void *msg, int32 staticVariables);
#endif

#if RETRO_REV02
inline void PrintText(int32 severity, const char *message) { PrintLog(severity, "%s", message); }
inline void PrintString(int32 severity, String *message)
{
    useEndLine = false;

    for (int32 c = 0; c < message->length; ++c) PrintLog(severity, "%c", message->chars[c]);
    PrintLog(severity, "\n");

    useEndLine = true;
}
inline void PrintUInt32(int32 severity, const char *message, uint integer) { PrintLog(severity, "%s: %d", message, integer); }
inline void PrintInt32(int32 severity, const char *message, int32 integer) { PrintLog(severity, "%s: %d", message, integer); }
inline void PrintFloat(int32 severity, const char *message, float f) { PrintLog(severity, "%s: %f", message, f); }
inline void PrintVector2(int32 severity, const char *message, int32 x, int32 y)
{
    PrintLog(severity, "%s: <%c%08X, %c%08X>", message, x < 0 ? 45 : 32, abs(x), y < 0 ? 45 : 32, abs(y));
}
inline void PrintHitbox(int32 severity, const char *message, RSDK::Hitbox *hitbox)
{
    PrintLog(severity, "%s: <l: %d, r: %d, t: %d, b: %d>", message, hitbox->left, hitbox->right, hitbox->top, hitbox->bottom);
}

struct DebugValueInfo {
    char name[0x10];
    void *value;
    int32 type;
    int32 size;
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

typedef enum {
    DEBUGDISPLAY_BOOL,
    DEBUGDISPLAY_UNSIGNED,
    DEBUGDISPLAY_SIGNED,
} DebugVarDisplayTypes;

extern int32 debugValueCount;
extern DebugValueInfo debugValues[DEBUGVALUE_LIST_COUNT];

inline void ClearDebugValues() { debugValueCount = 0; }
void SetDebugValue(const char *name, void *valPtr, int32 type, int32 min, int32 max);
#endif

struct DevMenu {
    void (*state)(void);
    int32 selection;
    int32 scrollPos;
    int32 timer;
    bool32 windowed;
    int8 sceneState;
    int8 listPos;
    int8 windowScale;
    int8 windowAspect;
#if RETRO_USE_MOD_LOADER
    bool32 modsChanged;
#endif
};

extern DevMenu devMenu;

void DevMenu_MainMenu();
void DevMenu_CategorySelectMenu();
void DevMenu_SceneSelectMenu();
void DevMenu_OptionsMenu();
void DevMenu_VideoOptionsMenu();
void DevMenu_AudioOptionsMenu();
void DevMenu_InputOptionsMenu();
void DevMenu_KeyMappingsMenu();
#if RETRO_REV02
void DevMenu_DebugOptionsMenu();
#endif
#if RETRO_USE_MOD_LOADER
void DevMenu_ModsMenu();
#endif

inline void OpenDevMenu()
{
    devMenu.sceneState = sceneInfo.state;
    devMenu.state      = DevMenu_MainMenu;
    devMenu.selection  = 0;
    devMenu.scrollPos  = 0;
    devMenu.timer      = 0;

    RSDK::videoSettings.screenCount = sceneInfo.state == ENGINESTATE_VIDEOPLAYBACK ? 1 : RSDK::videoSettings.screenCount;
    sceneInfo.state                 = ENGINESTATE_DEVMENU;
    PauseSound();
}

inline void CloseDevMenu()
{
    sceneInfo.state = devMenu.sceneState;

    RSDK::videoSettings.screenCount = sceneInfo.state == ENGINESTATE_VIDEOPLAYBACK ? 0 : RSDK::videoSettings.screenCount;
    ResumeSound();
}

#endif
