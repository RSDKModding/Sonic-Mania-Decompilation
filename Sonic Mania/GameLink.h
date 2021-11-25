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

typedef uint32 colour;
typedef uint32 color;

#define SCREEN_XMAX    (1280)
#define SCREEN_YSIZE   (240)
#define SCREEN_YCENTER (SCREEN_YSIZE / 2)

#define LAYER_COUNT     (8)
#define DRAWLAYER_COUNT (16)

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
    uint16 objectID;
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
    uint16 objectID;
    uint8 active;
} Object;

#define RSDK_OBJECT                                                                                                                                  \
    uint16 objectID;                                                                                                                                 \
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
    uint16 objectID;                                                                                                                                 \
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
    uint16 objectID;                                                                                                                                 \
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
#define Unknown_pausePress UnknownInfo->pausePress
#define Unknown_anyPress   UnknownInfo->anyPress

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
    bool32 anyPress;
    int32 unknown10;
} RSDKUnknownInfo;
#else
#define Unknown_pausePress TouchInfo->pausePress
#define Unknown_anyPress   TouchInfo->anyPress
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
} RSDKControllerState;

typedef struct {
    InputState keyUp;
    InputState keyDown;
    InputState keyLeft;
    InputState keyRight;
    InputState keyStick;
    float deadzone;
    float hDelta;
    float vDelta;
} RSDKAnalogState;

#if RETRO_USE_PLUS
typedef struct {
    InputState key1;
    InputState key2;
    float unknown1;
    float unknown2;
} RSDKTriggerState;
#endif

typedef struct {
    float x[0x10];
    float y[0x10];
    bool32 down[0x10];
    uint8 count;
#if !RETRO_USE_PLUS
    bool32 pausePressActive;
    bool32 pausePress;
    bool32 anyPressActive;
    bool32 anyPress;
    int32 unknown1;
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

typedef struct {
    void *functionPtrs;
#if RETRO_USE_PLUS
    void *APIPtrs;
    RSDKSKUInfo *skuInfo;
#endif
    RSDKGameInfo *gameInfo;
    RSDKSceneInfo *sceneInfo;
    RSDKControllerState *controllerInfo;
    RSDKAnalogState *stickInfoL;
#if RETRO_USE_PLUS
    RSDKAnalogState *stickInfoR;
    RSDKTriggerState *triggerInfoL;
    RSDKTriggerState *triggerInfoR;
#endif
    RSDKTouchInfo *touchInfo;
#if RETRO_USE_PLUS
    RSDKUnknownInfo *unknownInfo;
#endif
    RSDKScreenInfo *screenInfo;
#if RETRO_USE_MOD_LOADER
    void *modPtrs;
#endif
} EngineInfo;

typedef struct {
    int32 values[4][4];
} Matrix;

typedef struct {
    uint16 *text;
    uint16 textLength;
    uint16 length;
} TextInfo;

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
    SpriteFrame *framePtrs;
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
    TextInfo username;
#if RETRO_USE_PLUS
    TextInfo userID;
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
    PLATFORM_DEV    = 0xFF,
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
    INK_LOOKUP,
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
    DEVICE_TYPE_UNKNOWN    = 0,
    DEVICE_TYPE_KEYBOARD   = 1,
    DEVICE_TYPE_CONTROLLER = 2,
} InputDeviceTypes;

typedef enum {
    DEVICE_KEYBOARD        = 0,
    DEVICE_XBOX            = 1,
    DEVICE_PS4             = 2,
    DEVICE_SATURN          = 3,
    DEVICE_SWITCH          = 4,
    DEVICE_SWITCH_PRO      = 5,
    DEVICE_SWITCH_JOY_L    = 6,
    DEVICE_SWITCH_JOY_R    = 7,
    DEVICE_SWITCH_JOY_GRIP = 8,
} InputDeviceIDs;

typedef enum {
    ALIGN_LEFT,
    ALIGN_RIGHT,
    ALIGN_CENTER,
} Alignments;

typedef enum {
    PRINT_NORMAL,
    PRINT_POPUP,
    PRINT_ERROR,
    PRINT_FATAL,
} SeverityModes;

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
    VAR_UNKNOWN,
    VAR_COLOUR,
} VarTypes;

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
    S3D_UNKNOWN_2                       = 0x2,
    S3D_UNKNOWN_3                       = 0x3,
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
    SETTINGS_WINDOWED,
    SETTINGS_BORDERED,
    SETTINGS_EXCLUSIVEFS,
    SETTINGS_VSYNC,
    SETTINGS_TRIPLEBUFFERED,
    SETTINGS_WINDOW_WIDTH,
    SETTINGS_WINDOW_HEIGHT,
    SETTINGS_FSWIDTH,
    SETTINGS_FSHEIGHT,
    SETTINGS_REFRESHRATE,
    SETTINGS_SHADERSUPPORT,
    SETTINGS_SHADERID,
    SETTINGS_SCREENCOUNT,
#if RETRO_USE_PLUS
    SETTINGS_DIMTIMER,
#endif
    SETTINGS_STREAMSENABLED,
    SETTINGS_STREAM_VOL,
    SETTINGS_SFX_VOL,
    SETTINGS_LANGUAGE,
    SETTINGS_STORE,
    SETTINGS_RELOAD,
    SETTINGS_CHANGED,
    SETTINGS_WRITE,
} SettingsValues;

#if RETRO_USE_PLUS
typedef enum {
    SECRET_RICKYMODE = 0,
    SECRET_SUPERDASH = 1,
} GameCheats;
#endif

typedef enum {
    GROUP_ALL     = 0,
    GROUP_CUSTOM1 = TYPE_COUNT,
    GROUP_CUSTOM2,
    GROUP_CUSTOM3,
    GROUP_CUSTOM4,
} TypeGroups;

typedef enum {
    STATUS_NONE      = 0,
    STATUS_CONTINUE  = 100,
    STATUS_OK        = 200,
    STATUS_FORBIDDEN = 403,
    STATUS_NOTFOUND  = 404,
    STATUS_ERROR     = 500,
    STATUS_NOWIFI    = 503,
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
    ENGINESTATE_LOAD             = 0,
    ENGINESTATE_REGULAR          = 1,
    ENGINESTATE_PAUSED           = 2,
    ENGINESTATE_FROZEN           = 3,
    ENGINESTATE_LOAD_STEPOVER    = 4,
    ENGINESTATE_REGULAR_STEPOVER = 5,
    ENGINESTATE_PAUSED_STEPOVER  = 6,
    ENGINESTATE_FROZEN_STEPOVER  = 7,
    ENGINESTATE_DEVMENU          = 8,
    ENGINESTATE_VIDEOPLAYBACK    = 9,
    ENGINESTATE_SHOWPNG          = 0x0A,
#if RETRO_USE_PLUS
    ENGINESTATE_ERRORMSG       = 0x0B,
    ENGINESTATE_ERRORMSG_FATAL = 0x0C,
#endif
    ENGINESTATE_NULL = 0x0D,
} EngineStates;

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
