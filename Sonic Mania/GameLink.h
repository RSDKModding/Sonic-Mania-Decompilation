#ifndef GAMELINK_H
#define GAMELINK_H

// ================
// STANDARD LIBS
// ================
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

// ================
// STANDARD TYPES
// ================

typedef signed char int8;
typedef unsigned char uint8;
typedef signed short int16;
typedef unsigned short uint16;
typedef signed int int32;
typedef unsigned int uint32;

typedef uint32 bool32;
#define true 1
#define false 0

typedef uint32 color;

#define SCREEN_XMAX    (1280)
#define SCREEN_YSIZE   (240)
#define SCREEN_YCENTER (SCREEN_YSIZE / 2)

#define LAYER_COUNT     (8)
#define DRAWGROUP_COUNT (16)

#if RETRO_USE_PLUS
#define SCREEN_MAX (4)
#else
#define SCREEN_MAX (2)
#endif

#define PLAYER_MAX (4)
#define CAMERA_MAX (4)

// 0x800 scene objects, 0x40 reserved ones, and 0x100 spare slots for creation
#define RESERVE_ENTITY_COUNT (0x40)
#define TEMPENTITY_COUNT     (0x100)
#define SCENEENTITY_COUNT    (0x800)
#define ENTITY_COUNT         (RESERVE_ENTITY_COUNT + SCENEENTITY_COUNT + TEMPENTITY_COUNT)
#define TEMPENTITY_START     (ENTITY_COUNT - TEMPENTITY_COUNT)

#define TYPE_COUNT      (0x100)
#define TYPEGROUP_COUNT (0x104)

#define CHANNEL_COUNT (0x10)

#define TILE_SIZE (16)

typedef struct {
    int32 x;
    int32 y;
} Vector2;

typedef struct {
    Vector2 position;
    Vector2 scale;
    Vector2 velocity;
    Vector2 updateRange;
    int32 angle;
    int32 alpha;
    int32 rotation;
    int32 groundVel;
    int32 depth;
    uint16 group;
    uint16 classID;
    bool32 inBounds;
    bool32 isPermanent;
    bool32 tileCollisions;
    bool32 interaction;
    bool32 onGround;
    uint8 active;
#if RETRO_USE_PLUS
    uint8 filter;
#endif
    uint8 direction;
    uint8 drawOrder;
    uint8 collisionLayers;
    uint8 collisionPlane;
    uint8 collisionMode;
    uint8 drawFX;
    uint8 inkEffect;
    uint8 visible;
    uint8 activeScreens;
} Entity;

typedef struct {
    uint16 classID;
    uint8 active;
} Object;

#define RSDK_OBJECT                                                                                                                                  \
    uint16 classID;                                                                                                                                 \
    uint8 active;

#if RETRO_USE_PLUS
#define RSDK_ENTITY                                                                                                                                  \
    Vector2 position;                                                                                                                                \
    Vector2 scale;                                                                                                                                   \
    Vector2 velocity;                                                                                                                                \
    Vector2 updateRange;                                                                                                                             \
    int32 angle;                                                                                                                                     \
    int32 alpha;                                                                                                                                     \
    int32 rotation;                                                                                                                                  \
    int32 groundVel;                                                                                                                                 \
    int32 depth3D;                                                                                                                                   \
    uint16 group;                                                                                                                                    \
    uint16 classID;                                                                                                                                 \
    bool32 inBounds;                                                                                                                                 \
    bool32 isPermanent;                                                                                                                              \
    bool32 tileCollisions;                                                                                                                           \
    bool32 interaction;                                                                                                                              \
    bool32 onGround;                                                                                                                                 \
    uint8 active;                                                                                                                                    \
    uint8 filter;                                                                                                                                    \
    uint8 direction;                                                                                                                                 \
    uint8 drawOrder;                                                                                                                                 \
    uint8 collisionLayers;                                                                                                                           \
    uint8 collisionPlane;                                                                                                                            \
    uint8 collisionMode;                                                                                                                             \
    uint8 drawFX;                                                                                                                                    \
    uint8 inkEffect;                                                                                                                                 \
    uint8 visible;                                                                                                                                   \
    uint8 activeScreens;
#else
#define RSDK_ENTITY                                                                                                                                  \
    Vector2 position;                                                                                                                                \
    Vector2 scale;                                                                                                                                   \
    Vector2 velocity;                                                                                                                                \
    Vector2 updateRange;                                                                                                                             \
    int32 angle;                                                                                                                                     \
    int32 alpha;                                                                                                                                     \
    int32 rotation;                                                                                                                                  \
    int32 groundVel;                                                                                                                                 \
    int32 depth3D;                                                                                                                                   \
    uint16 group;                                                                                                                                    \
    uint16 classID;                                                                                                                                 \
    bool32 inBounds;                                                                                                                                 \
    bool32 isPermanent;                                                                                                                              \
    bool32 tileCollisions;                                                                                                                           \
    bool32 interaction;                                                                                                                              \
    bool32 onGround;                                                                                                                                 \
    uint8 active;                                                                                                                                    \
    uint8 direction;                                                                                                                                 \
    uint8 drawOrder;                                                                                                                                 \
    uint8 collisionLayers;                                                                                                                           \
    uint8 collisionPlane;                                                                                                                            \
    uint8 collisionMode;                                                                                                                             \
    uint8 drawFX;                                                                                                                                    \
    uint8 inkEffect;                                                                                                                                 \
    uint8 visible;                                                                                                                                   \
    uint8 activeScreens;
#endif

#define ENTITY_SIZE (sizeof(Entity) + (0x100 * sizeof(void *)))

#if RETRO_USE_PLUS
#define Unknown_pausePress  UnknownInfo->pausePress
#define Unknown_anyKeyPress UnknownInfo->anyKeyPress

typedef struct {
    int32 platform;
    int32 language;
    int32 region;
} RSDKSKUInfo;

// None of these besides the named 2 are even used
// and even then they're not even set in plus
typedef struct {
    int32 unknown1;
    int32 unknown2;
    int32 unknown3;
    int32 unknown4;
    bool32 pausePress;
    int32 unknown5;
    int32 unknown6;
    int32 unknown7;
    int32 unknown8;
    int32 unknown9;
    bool32 anyKeyPress;
    int32 unknown10;
} RSDKUnknownInfo;
#else
#define Unknown_pausePress  TouchInfo->pausePress
#define Unknown_anyKeyPress TouchInfo->anyKeyPress
#endif

typedef struct {
    char engineInfo[0x40];
    char gameSubname[0x100];
    char version[0x10];
#if !RETRO_USE_PLUS
    uint8 platform;
    uint8 language;
    uint8 region;
#endif
} RSDKGameInfo;

typedef struct {
    Entity *entity;
    void *listData;
    void *listCategory;
    int32 timeCounter;
    int32 currentDrawGroup;
    int32 currentScreenID;
    uint16 listPos;
    uint16 entitySlot;
    uint16 createSlot;
    uint16 classCount;
    bool32 inEditor;
    bool32 effectGizmo;
    bool32 debugMode;
    bool32 useGlobalScripts;
    bool32 timeEnabled;
    uint8 activeCategory;
    uint8 categoryCount;
    uint8 state;
#if RETRO_USE_PLUS
    uint8 filter;
#endif
    uint8 milliseconds;
    uint8 seconds;
    uint8 minutes;
} RSDKSceneInfo;

typedef struct {
    bool32 down;
    bool32 press;
    int32 keyMap;
} InputState;

typedef struct {
    InputState keyUp;
    InputState keyDown;
    InputState keyLeft;
    InputState keyRight;
    InputState keyA;
    InputState keyB;
    InputState keyC;
    InputState keyX;
    InputState keyY;
    InputState keyZ;
    InputState keyStart;
    InputState keySelect;

    // Rev01 hasn't split these into different structs yet
#if !RETRO_USE_PLUS
    InputState bumperL;
    InputState bumperR;
    InputState keyTriggerL;
    InputState keyTriggerR;
    InputState stickL;
    InputState stickR;
#endif
} RSDKControllerState;

typedef struct {
    InputState keyUp;
    InputState keyDown;
    InputState keyLeft;
    InputState keyRight;
#if RETRO_USE_PLUS
    InputState keyStick;
    float deadzone;
    float hDelta;
    float vDelta;
#else
    float deadzone;
    float triggerDeltaL;
    float triggerDeltaR;
    float hDeltaL;
    float vDeltaL;
    float hDeltaR;
    float vDeltaR;
#endif
} RSDKAnalogState;

#if RETRO_USE_PLUS
typedef struct {
    InputState keyBumper;
    InputState keyTrigger;
    float deadzone;
    float delta;
} RSDKTriggerState;
#endif

typedef struct {
    float x[0x10];
    float y[0x10];
    bool32 down[0x10];
    uint8 count;
#if !RETRO_USE_PLUS
    bool32 pauseHold;
    bool32 pausePress;
    bool32 unknown1;
    bool32 anyKeyHold;
    bool32 anyKeyPress;
    bool32 unknown2;
#endif
} RSDKTouchInfo;

typedef struct {
    // uint16 *frameBuffer;
    uint16 frameBuffer[SCREEN_XMAX * SCREEN_YSIZE];
    Vector2 position;
    int32 width;
    int32 height;
    int32 centerX;
    int32 centerY;
    int32 pitch;
    int32 clipBound_X1;
    int32 clipBound_Y1;
    int32 clipBound_X2;
    int32 clipBound_Y2;
    int32 waterDrawPos;
} RSDKScreenInfo;

#if RETRO_USE_PLUS
typedef struct {
    void *functionTable;
    void *APITable;

    RSDKGameInfo *gameInfo;
    RSDKSKUInfo *currentSKU;
    RSDKSceneInfo *sceneInfo;

    RSDKControllerState *controllerInfo;
    RSDKAnalogState *stickInfoL;
    RSDKAnalogState *stickInfoR;
    RSDKTriggerState *triggerInfoL;
    RSDKTriggerState *triggerInfoR;
    RSDKTouchInfo *touchInfo;

    RSDKUnknownInfo *unknownInfo;

    RSDKScreenInfo *screenInfo;

#if RETRO_USE_MOD_LOADER
    void *modTable;
#endif
} EngineInfo;
#else
typedef struct {
    void *functionTable;

    RSDKGameInfo *gameInfo;
    RSDKSceneInfo *sceneInfo;

    RSDKControllerState *controllerInfo;
    RSDKAnalogState *stickInfoL;

    RSDKTouchInfo *touchInfo;

    RSDKScreenInfo *screenInfo;

#if RETRO_USE_MOD_LOADER
    void *modTable;
#endif
} EngineInfo;
#endif

typedef struct {
    int32 values[4][4];
} Matrix;

typedef struct {
    uint16 *chars;
    uint16 length;
    uint16 size;
} String;

typedef struct {
    int16 left;
    int16 top;
    int16 right;
    int16 bottom;
} Hitbox;

typedef struct {
    int16 sprX;
    int16 sprY;
    int16 width;
    int16 height;
    int16 pivotX;
    int16 pivotY;
    uint16 delay;
    int16 id;
    uint8 sheetID;
    uint8 hitboxCnt;
    Hitbox hitboxes[8];
} SpriteFrame;

typedef struct {
    SpriteFrame *frames;
    int32 frameID;
    int16 animationID;
    int16 prevAnimationID;
    int16 speed;
    int16 timer;
    int16 frameDuration;
    int16 frameCount;
    uint8 loopIndex;
    uint8 rotationFlag;
} Animator;

typedef struct {
    int32 tilePos;
    int32 parallaxFactor;
    int32 scrollSpeed;
    int32 scrollPos;
    uint8 deform;
    uint8 unknown;
} ScrollInfo;

typedef struct {
    Vector2 position;
    Vector2 deform;
} ScanlineInfo;

typedef struct {
    uint8 behaviour;
    uint8 drawLayer[4];
    uint8 widthShift;
    uint8 heightShift;
    uint16 width;
    uint16 height;
    Vector2 position;
    int32 parallaxFactor;
    int32 scrollSpeed;
    int32 scrollPos;
    int32 deformationOffset;
    int32 deformationOffsetW;
    int32 deformationData[0x400];
    int32 deformationDataW[0x400];
    void (*scanlineCallback)(ScanlineInfo *);
    uint16 scrollInfoCount;
    ScrollInfo scrollInfo[0x100];
    uint32 name[4];
    uint16 *layout;
    uint8 *lineScroll;
} TileLayer;

typedef struct {
    int id1;
    int id2;
    int id_switch;
    const char *name_legacy;
    const char *name;
} LeaderboardID;

typedef struct {
    String username;
#if RETRO_USE_PLUS
    String userID;
#endif
    int32 globalRank;
    int32 score;
    bool32 isUser;
    int32 status;
} LeaderboardEntry;

typedef struct {
    uint8 statID;
    const char *name;
    void *data[64];
} StatInfo;

typedef enum {
    PLATFORM_PC     = 0,
    PLATFORM_PS4    = 1,
    PLATFORM_XB1    = 2,
    PLATFORM_SWITCH = 3,

    PLATFORM_DEV = 0xFF,
} GamePlatforms;

typedef enum {
    SCOPE_NONE,
    SCOPE_GLOBAL,
    SCOPE_STAGE,
} Scopes;

typedef enum {
    INK_NONE,
    INK_BLEND,
    INK_ALPHA,
    INK_ADD,
    INK_SUB,
    INK_TINT,
    INK_MASKED,
    INK_UNMASKED,
} InkEffects;

typedef enum { FX_NONE = 0, FX_FLIP = 1, FX_ROTATE = 2, FX_SCALE = 4 } DrawFX;

typedef enum { FLIP_NONE, FLIP_X, FLIP_Y, FLIP_XY } FlipFlags;

typedef enum { TYPE_BLANK } DefaultObjTypes;

typedef enum {
    CONT_UNASSIGNED = -2,
    CONT_AUTOASSIGN = -1,
    CONT_ANY        = 0,
    CONT_P1         = 1,
    CONT_P2         = 2,
    CONT_P3         = 3,
    CONT_P4         = 4,
} ControllerIDs;

typedef enum {
    DEVICE_TYPE_NONE,
    DEVICE_TYPE_KEYBOARD,
    DEVICE_TYPE_CONTROLLER,
    DEVICE_TYPE_UNKNOWN,
    DEVICE_TYPE_STEAMOVERLAY,
} InputDeviceTypes;

typedef enum {
    DEVICE_KEYBOARD,
    DEVICE_XBOX,
    DEVICE_PS4,
    DEVICE_SATURN,
    DEVICE_SWITCH_HANDHELD,
    DEVICE_SWITCH_JOY_GRIP,
    DEVICE_SWITCH_JOY_L,
    DEVICE_SWITCH_JOY_R,
    DEVICE_SWITCH_PRO,
} InputDeviceIDs;

typedef enum {
    DEVICE_API_NONE,
    DEVICE_API_KEYBOARD,
    DEVICE_API_XINPUT,
    DEVICE_API_RAWINPUT,
    DEVICE_API_STEAM,
} InputDeviceAPIs;

typedef enum {
    ALIGN_LEFT,
    ALIGN_RIGHT,
    ALIGN_CENTER,
} Alignments;

#if RETRO_USE_PLUS
typedef enum {
    PRINT_NORMAL,
    PRINT_POPUP,
    PRINT_ERROR,
    PRINT_FATAL,
} PrintModes;
#else
typedef enum {
    MESSAGE_STRING,
    MESSAGE_INT32,
    MESSAGE_UINT32,
    MESSAGE_FLOAT,
} PrintMessageTypes;
#endif

typedef enum {
    VAR_UINT8,
    VAR_UINT16,
    VAR_UINT32,
    VAR_INT8,
    VAR_INT16,
    VAR_INT32,
    VAR_ENUM,
    VAR_BOOL,
    VAR_STRING,
    VAR_VECTOR2,
    VAR_FLOAT,
    VAR_COLOR,
} VarTypes;

#if RETRO_USE_PLUS
typedef enum {
    DBVAR_UNKNOWN, // unused (in Sonic Mania)
    DBVAR_BOOL,    // unused (in Sonic Mania)
    DBVAR_UINT8,   // used (in Sonic Mania)
    DBVAR_UINT16,  // unused (in Sonic Mania)
    DBVAR_UINT32,  // used (in Sonic Mania)
    DBVAR_UINT64,  // unimplemented in RSDKv5
    DBVAR_INT8,    // unused (in Sonic Mania)
    DBVAR_INT16,   // unused (in Sonic Mania)
    DBVAR_INT32,   // unused (in Sonic Mania)
    DBVAR_INT64,   // unimplemented in RSDKv5
    DBVAR_FLOAT,   // unused (in Sonic Mania)
    DBVAR_DOUBLE,  // unimplemented in RSDKv5
    DBVAR_VECTOR2, // unimplemented in RSDKv5
    DBVAR_VECTOR3, // unimplemented in RSDKv5
    DBVAR_VECTOR4, // unimplemented in RSDKv5
    DBVAR_COLOR,   // unused (in Sonic Mania)
    DBVAR_STRING,  // unused (in Sonic Mania)
    DBVAR_HASHMD5, // unimplemented in RSDKv5
} DBVarTypes;

typedef enum {
    DTYPE_BOOL = 1,
    DTYPE_UINT8,
    DTYPE_UINT16,
    DTYPE_UINT32,
    DTYPE_INT8,
    DTYPE_INT16,
    DTYPE_INT32,
} DebugVarTypes;
#endif

typedef enum {
    ACTIVE_NEVER,
    ACTIVE_ALWAYS,
    ACTIVE_NORMAL,
    ACTIVE_PAUSED,
    ACTIVE_BOUNDS,
    ACTIVE_XBOUNDS,
    ACTIVE_YBOUNDS,
    ACTIVE_RBOUNDS,
    ACTIVE_NEVER2 = 0xFF,
} ActiveFlags;

typedef enum {
    LAYER_HSCROLL,
    LAYER_VSCROLL,
    LAYER_ROTOZOOM,
    LAYER_BASIC,
} LayerTypes;

typedef enum {
    CMODE_FLOOR,
    CMODE_LWALL,
    CMODE_ROOF,
    CMODE_RWALL,
} CModes;

typedef enum {
    C_NONE,
    C_TOP,
    C_LEFT,
    C_RIGHT,
    C_BOTTOM,
} CSides;

typedef enum {
    S3D_FLATCLR_WIREFRAME               = 0x0,
    S3D_FLATCLR                         = 0x1,
    S3D_UNUSED_1                        = 0x2,
    S3D_UNUSED_2                        = 0x3,
    S3D_FLATCLR_SHADED_WIREFRAME        = 0x4,
    S3D_FLATCLR_SHADED                  = 0x5,
    S3D_FLATCLR_SHADED_BLENDED          = 0x6,
    S3D_FLATCLR_SCREEN_WIREFRAME        = 0x7,
    S3D_FLATCLR_SCREEN                  = 0x8,
    S3D_FLATCLR_SHADED_SCREEN_WIREFRAME = 0x9,
    S3D_FLATCLR_SHADED_SCREEN           = 0xA,
    S3D_FLATCLR_SHADED_BLENDED_SCREEN   = 0xB,
} Scene3DDrawTypes;

typedef enum {
    VIDEOSETTING_WINDOWED,
    VIDEOSETTING_BORDERED,
    VIDEOSETTING_EXCLUSIVEFS,
    VIDEOSETTING_VSYNC,
    VIDEOSETTING_TRIPLEBUFFERED,
    VIDEOSETTING_WINDOW_WIDTH,
    VIDEOSETTING_WINDOW_HEIGHT,
    VIDEOSETTING_FSWIDTH,
    VIDEOSETTING_FSHEIGHT,
    VIDEOSETTING_REFRESHRATE,
    VIDEOSETTING_SHADERSUPPORT,
    VIDEOSETTING_SHADERID,
    VIDEOSETTING_SCREENCOUNT,
#if RETRO_USE_PLUS
    VIDEOSETTING_DIMTIMER,
#endif
    VIDEOSETTING_STREAMSENABLED,
    VIDEOSETTING_STREAM_VOL,
    VIDEOSETTING_SFX_VOL,
    VIDEOSETTING_LANGUAGE,
    VIDEOSETTING_STORE,
    VIDEOSETTING_RELOAD,
    VIDEOSETTING_CHANGED,
    VIDEOSETTING_WRITE,
} VideoSettings;

typedef enum {
    GROUP_ALL     = 0,

    GROUP_CUSTOM0 = TYPE_COUNT,
    GROUP_CUSTOM1,
    GROUP_CUSTOM2,
    GROUP_CUSTOM3,
} TypeGroups;

typedef enum {
    STATUS_NONE      = 0,
    STATUS_CONTINUE  = 100,
    STATUS_OK        = 200,
    STATUS_FORBIDDEN = 403,
    STATUS_NOTFOUND  = 404,
    STATUS_ERROR     = 500,
    STATUS_NOWIFI    = 503,
    STATUS_TIMEOUT   = 504,
    STATUS_CORRUPT   = 505,
    STATUS_NOSPACE   = 506,
} StatusCodes;

typedef enum {
    REGION_US,
    REGION_JP,
    REGION_EU,
} GameRegions;

typedef enum {
    LANGUAGE_EN,
    LANGUAGE_FR,
    LANGUAGE_IT,
    LANGUAGE_GE,
    LANGUAGE_SP,
    LANGUAGE_JP,
#if RETRO_GAMEVER != VER_100
    LANGUAGE_KO,
    LANGUAGE_SC,
    LANGUAGE_TC,
#endif
} GameLanguages;

typedef enum {
    ENGINESTATE_LOAD,
    ENGINESTATE_REGULAR,
    ENGINESTATE_PAUSED,
    ENGINESTATE_FROZEN,
    ENGINESTATE_STEPOVER = 4,
    ENGINESTATE_DEVMENU  = 8,
    ENGINESTATE_VIDEOPLAYBACK,
    ENGINESTATE_SHOWIMAGE,
#if RETRO_USE_PLUS
    ENGINESTATE_ERRORMSG,
    ENGINESTATE_ERRORMSG_FATAL,
#endif
    ENGINESTATE_NONE,
} EngineStates;

// see: https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
// for value list & descriptions
typedef enum {
    KEYMAP_AUTO_MAPPING                    = -1,
    KEYMAP_NO_MAPPING                      = 0,
    KEYMAP_LBUTTON                         = 0x01,
    KEYMAP_RBUTTON                         = 0x02,
    KEYMAP_CANCEL                          = 0x03,
    KEYMAP_MBUTTON                         = 0x04,
    KEYMAP_XBUTTON1                        = 0x05,
    KEYMAP_XBUTTON2                        = 0x06,
    KEYMAP_BACK                            = 0x08,
    KEYMAP_TAB                             = 0x09,
    KEYMAP_CLEAR                           = 0x0C,
    KEYMAP_RETURN                          = 0x0D,
    KEYMAP_SHIFT                           = 0x10,
    KEYMAP_CONTROL                         = 0x11,
    KEYMAP_MENU                            = 0x12,
    KEYMAP_PAUSE                           = 0x13,
    KEYMAP_CAPITAL                         = 0x14,
    KEYMAP_KANA                            = 0x15,
    KEYMAP_HANGEUL                         = 0x15,
    KEYMAP_HANGUL                          = 0x15,
    KEYMAP_JUNJA                           = 0x17,
    KEYMAP_FINAL                           = 0x18,
    KEYMAP_HANJA                           = 0x19,
    KEYMAP_KANJI                           = 0x19,
    KEYMAP_ESCAPE                          = 0x1B,
    KEYMAP_CONVERT                         = 0x1C,
    KEYMAP_NONCONVERT                      = 0x1D,
    KEYMAP_ACCEPT                          = 0x1E,
    KEYMAP_MODECHANGE                      = 0x1F,
    KEYMAP_SPACE                           = 0x20,
    KEYMAP_PRIOR                           = 0x21,
    KEYMAP_NEXT                            = 0x22,
    KEYMAP_END                             = 0x23,
    KEYMAP_HOME                            = 0x24,
    KEYMAP_LEFT                            = 0x25,
    KEYMAP_UP                              = 0x26,
    KEYMAP_RIGHT                           = 0x27,
    KEYMAP_DOWN                            = 0x28,
    KEYMAP_SELECT                          = 0x29,
    KEYMAP_PRINT                           = 0x2A,
    KEYMAP_EXECUTE                         = 0x2B,
    KEYMAP_SNAPSHOT                        = 0x2C,
    KEYMAP_INSERT                          = 0x2D,
    KEYMAP_DELETE                          = 0x2E,
    KEYMAP_HELP                            = 0x2F,
    KEYMAP_0                               = 0x30,
    KEYMAP_1                               = 0x31,
    KEYMAP_2                               = 0x32,
    KEYMAP_3                               = 0x33,
    KEYMAP_4                               = 0x34,
    KEYMAP_5                               = 0x35,
    KEYMAP_6                               = 0x36,
    KEYMAP_7                               = 0x37,
    KEYMAP_8                               = 0x38,
    KEYMAP_9                               = 0x39,
    KEYMAP_A                               = 0x41,
    KEYMAP_B                               = 0x42,
    KEYMAP_C                               = 0x43,
    KEYMAP_D                               = 0x44,
    KEYMAP_E                               = 0x45,
    KEYMAP_F                               = 0x46,
    KEYMAP_G                               = 0x47,
    KEYMAP_H                               = 0x48,
    KEYMAP_I                               = 0x49,
    KEYMAP_J                               = 0x4A,
    KEYMAP_K                               = 0x4B,
    KEYMAP_L                               = 0x4C,
    KEYMAP_M                               = 0x4D,
    KEYMAP_N                               = 0x4E,
    KEYMAP_O                               = 0x4F,
    KEYMAP_P                               = 0x50,
    KEYMAP_Q                               = 0x51,
    KEYMAP_R                               = 0x52,
    KEYMAP_S                               = 0x53,
    KEYMAP_T                               = 0x54,
    KEYMAP_U                               = 0x55,
    KEYMAP_V                               = 0x56,
    KEYMAP_W                               = 0x57,
    KEYMAP_X                               = 0x58,
    KEYMAP_Y                               = 0x59,
    KEYMAP_Z                               = 0x5A,
    KEYMAP_LWIN                            = 0x5B,
    KEYMAP_RWIN                            = 0x5C,
    KEYMAP_APPS                            = 0x5D,
    KEYMAP_SLEEP                           = 0x5F,
    KEYMAP_NUMPAD0                         = 0x60,
    KEYMAP_NUMPAD1                         = 0x61,
    KEYMAP_NUMPAD2                         = 0x62,
    KEYMAP_NUMPAD3                         = 0x63,
    KEYMAP_NUMPAD4                         = 0x64,
    KEYMAP_NUMPAD5                         = 0x65,
    KEYMAP_NUMPAD6                         = 0x66,
    KEYMAP_NUMPAD7                         = 0x67,
    KEYMAP_NUMPAD8                         = 0x68,
    KEYMAP_NUMPAD9                         = 0x69,
    KEYMAP_MULTIPLY                        = 0x6A,
    KEYMAP_ADD                             = 0x6B,
    KEYMAP_SEPARATOR                       = 0x6C,
    KEYMAP_SUBTRACT                        = 0x6D,
    KEYMAP_DECIMAL                         = 0x6E,
    KEYMAP_DIVIDE                          = 0x6F,
    KEYMAP_F1                              = 0x70,
    KEYMAP_F2                              = 0x71,
    KEYMAP_F3                              = 0x72,
    KEYMAP_F4                              = 0x73,
    KEYMAP_F5                              = 0x74,
    KEYMAP_F6                              = 0x75,
    KEYMAP_F7                              = 0x76,
    KEYMAP_F8                              = 0x77,
    KEYMAP_F9                              = 0x78,
    KEYMAP_F10                             = 0x79,
    KEYMAP_F11                             = 0x7A,
    KEYMAP_F12                             = 0x7B,
    KEYMAP_F13                             = 0x7C,
    KEYMAP_F14                             = 0x7D,
    KEYMAP_F15                             = 0x7E,
    KEYMAP_F16                             = 0x7F,
    KEYMAP_F17                             = 0x80,
    KEYMAP_F18                             = 0x81,
    KEYMAP_F19                             = 0x82,
    KEYMAP_F20                             = 0x83,
    KEYMAP_F21                             = 0x84,
    KEYMAP_F22                             = 0x85,
    KEYMAP_F23                             = 0x86,
    KEYMAP_F24                             = 0x87,
    KEYMAP_NAVIGATION_VIEW                 = 0x88,
    KEYMAP_NAVIGATION_MENU                 = 0x89,
    KEYMAP_NAVIGATION_UP                   = 0x8A,
    KEYMAP_NAVIGATION_DOWN                 = 0x8B,
    KEYMAP_NAVIGATION_LEFT                 = 0x8C,
    KEYMAP_NAVIGATION_RIGHT                = 0x8D,
    KEYMAP_NAVIGATION_ACCEPT               = 0x8E,
    KEYMAP_NAVIGATION_CANCEL               = 0x8F,
    KEYMAP_NUMLOCK                         = 0x90,
    KEYMAP_SCROLL                          = 0x91,
    KEYMAP_OEM_NEC_EQUAL                   = 0x92,
    KEYMAP_OEM_FJ_JISHO                    = 0x92,
    KEYMAP_OEM_FJ_MASSHOU                  = 0x93,
    KEYMAP_OEM_FJ_TOUROKU                  = 0x94,
    KEYMAP_OEM_FJ_LOYA                     = 0x95,
    KEYMAP_OEM_FJ_ROYA                     = 0x96,
    KEYMAP_LSHIFT                          = 0xA0,
    KEYMAP_RSHIFT                          = 0xA1,
    KEYMAP_LCONTROL                        = 0xA2,
    KEYMAP_RCONTROL                        = 0xA3,
    KEYMAP_LMENU                           = 0xA4,
    KEYMAP_RMENU                           = 0xA5,
    KEYMAP_BROWSER_BACK                    = 0xA6,
    KEYMAP_BROWSER_FORWARD                 = 0xA7,
    KEYMAP_BROWSER_REFRESH                 = 0xA8,
    KEYMAP_BROWSER_STOP                    = 0xA9,
    KEYMAP_BROWSER_SEARCH                  = 0xAA,
    KEYMAP_BROWSER_FAVORITES               = 0xAB,
    KEYMAP_BROWSER_HOME                    = 0xAC,
    KEYMAP_VOLUME_MUTE                     = 0xAD,
    KEYMAP_VOLUME_DOWN                     = 0xAE,
    KEYMAP_VOLUME_UP                       = 0xAF,
    KEYMAP_MEDIA_NEXT_TRACK                = 0xB0,
    KEYMAP_MEDIA_PREV_TRACK                = 0xB1,
    KEYMAP_MEDIA_STOP                      = 0xB2,
    KEYMAP_MEDIA_PLAY_PAUSE                = 0xB3,
    KEYMAP_LAUNCH_MAIL                     = 0xB4,
    KEYMAP_LAUNCH_MEDIA_SELECT             = 0xB5,
    KEYMAP_LAUNCH_APP1                     = 0xB6,
    KEYMAP_LAUNCH_APP2                     = 0xB7,
    KEYMAP_OEM_1                           = 0xBA,
    KEYMAP_OEM_PLUS                        = 0xBB,
    KEYMAP_OEM_COMMA                       = 0xBC,
    KEYMAP_OEM_MINUS                       = 0xBD,
    KEYMAP_OEM_PERIOD                      = 0xBE,
    KEYMAP_OEM_2                           = 0xBF,
    KEYMAP_OEM_3                           = 0xC0,
    KEYMAP_GAMEPAD_A                       = 0xC3,
    KEYMAP_GAMEPAD_B                       = 0xC4,
    KEYMAP_GAMEPAD_X                       = 0xC5,
    KEYMAP_GAMEPAD_Y                       = 0xC6,
    KEYMAP_GAMEPAD_RIGHT_SHOULDER          = 0xC7,
    KEYMAP_GAMEPAD_LEFT_SHOULDER           = 0xC8,
    KEYMAP_GAMEPAD_LEFT_TRIGGER            = 0xC9,
    KEYMAP_GAMEPAD_RIGHT_TRIGGER           = 0xCA,
    KEYMAP_GAMEPAD_DPAD_UP                 = 0xCB,
    KEYMAP_GAMEPAD_DPAD_DOWN               = 0xCC,
    KEYMAP_GAMEPAD_DPAD_LEFT               = 0xCD,
    KEYMAP_GAMEPAD_DPAD_RIGHT              = 0xCE,
    KEYMAP_GAMEPAD_MENU                    = 0xCF,
    KEYMAP_GAMEPAD_VIEW                    = 0xD0,
    KEYMAP_GAMEPAD_LEFT_THUMBSTICK_BUTTON  = 0xD1,
    KEYMAP_GAMEPAD_RIGHT_THUMBSTICK_BUTTON = 0xD2,
    KEYMAP_GAMEPAD_LEFT_THUMBSTICK_UP      = 0xD3,
    KEYMAP_GAMEPAD_LEFT_THUMBSTICK_DOWN    = 0xD4,
    KEYMAP_GAMEPAD_LEFT_THUMBSTICK_RIGHT   = 0xD5,
    KEYMAP_GAMEPAD_LEFT_THUMBSTICK_LEFT    = 0xD6,
    KEYMAP_GAMEPAD_RIGHT_THUMBSTICK_UP     = 0xD7,
    KEYMAP_GAMEPAD_RIGHT_THUMBSTICK_DOWN   = 0xD8,
    KEYMAP_GAMEPAD_RIGHT_THUMBSTICK_RIGHT  = 0xD9,
    KEYMAP_GAMEPAD_RIGHT_THUMBSTICK_LEFT   = 0xDA,
    KEYMAP_OEM_4                           = 0xDB,
    KEYMAP_OEM_5                           = 0xDC,
    KEYMAP_OEM_6                           = 0xDD,
    KEYMAP_OEM_7                           = 0xDE,
    KEYMAP_OEM_8                           = 0xDF,
    KEYMAP_OEM_AX                          = 0xE1,
    KEYMAP_OEM_102                         = 0xE2,
    KEYMAP_ICO_HELP                        = 0xE3,
    KEYMAP_ICO_00                          = 0xE4,
    KEYMAP_PROCESSKEY                      = 0xE5,
    KEYMAP_ICO_CLEAR                       = 0xE6,
    KEYMAP_PACKET                          = 0xE7,
    KEYMAP_OEM_RESET                       = 0xE9,
    KEYMAP_OEM_JUMP                        = 0xEA,
    KEYMAP_OEM_PA1                         = 0xEB,
    KEYMAP_OEM_PA2                         = 0xEC,
    KEYMAP_OEM_PA3                         = 0xED,
    KEYMAP_OEM_WSCTRL                      = 0xEE,
    KEYMAP_OEM_CUSEL                       = 0xEF,
    KEYMAP_OEM_ATTN                        = 0xF0,
    KEYMAP_OEM_FINISH                      = 0xF1,
    KEYMAP_OEM_COPY                        = 0xF2,
    KEYMAP_OEM_AUTO                        = 0xF3,
    KEYMAP_OEM_ENLW                        = 0xF4,
    KEYMAP_OEM_BACKTAB                     = 0xF5,
    KEYMAP_ATTN                            = 0xF6,
    KEYMAP_CRSEL                           = 0xF7,
    KEYMAP_EXSEL                           = 0xF8,
    KEYMAP_EREOF                           = 0xF9,
    KEYMAP_PLAY                            = 0xFA,
    KEYMAP_ZOOM                            = 0xFB,
    KEYMAP_NONAME                          = 0xFC,
    KEYMAP_PA1                             = 0xFD,
    KEYMAP_OEM_CLEAR                       = 0xFE,
} KeyMappings;

// Macros and other handy things

// used for casts and etc
#define Type_StateMachine void (*)(void)
// used for variable decl
#define StateMachine(name) void (*name)(void)
#define StateMachine_Run(func)                                                                                                                       \
    if (func) {                                                                                                                                      \
        func();                                                                                                                                      \
    }
#define StateMachine_None NULL

// Fancy macro + build magic to make tables & static vars
#define TABLE(var, ...)  var
#define STATIC(var, val) var

#endif /* GAMELINK_H */
