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

typedef signed char sbyte;
typedef unsigned char byte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef long long int64;
typedef unsigned long long uint64;
typedef unsigned int bool32;

#define true 1
#define false 0

typedef uint colour;
typedef uint color;

#define SCREEN_XMAX     (1280)
#define SCREEN_YSIZE    (240)
#define SCREEN_YCENTER  (SCREEN_YSIZE / 2)

#define LAYER_COUNT     (8)
#define DRAWLAYER_COUNT (16)

#if RETRO_USE_PLUS
#define SCREEN_MAX (4)
#else
#define SCREEN_MAX (2)
#endif

#define PLAYER_MAX (4)
#define CAMERA_MAX (4)

//0x800 scene objects, 0x40 reserved ones, and 0x100 spare slots for creation
#define RESERVE_ENTITY_COUNT (0x40)
#define TEMPENTITY_COUNT     (0x100)
#define SCENEENTITY_COUNT    (0x800)
#define ENTITY_COUNT         (RESERVE_ENTITY_COUNT + SCENEENTITY_COUNT + TEMPENTITY_COUNT)
#define TEMPENTITY_START     (ENTITY_COUNT - TEMPENTITY_COUNT)

#define TYPE_COUNT        (0x100)
#define TYPEGROUP_COUNT   (0x104)

#define CHANNEL_COUNT (0x10)

#define TILE_SIZE (16)

typedef struct {
    int x;
    int y;
} Vector2;

typedef struct {
    Vector2 position;
    Vector2 scale;
    Vector2 velocity;
    Vector2 updateRange;
    int angle;
    int alpha;
    int rotation;
    int groundVel;
    int depth;
    ushort group;
    ushort objectID;
    bool32 inBounds;
    bool32 isPermanent;
    bool32 tileCollisions;
    bool32 interaction;
    bool32 onGround;
    byte active;
#if RETRO_USE_PLUS
    byte filter;
#endif
    byte direction;
    byte drawOrder;
    byte collisionLayers;
    byte collisionPlane;
    byte collisionMode;
    byte drawFX;
    byte inkEffect;
    byte visible;
    byte activeScreens;
} Entity;

typedef struct {
    ushort objectID;
    byte active;
} Object;

#define RSDK_OBJECT                                                                                                                                  \
    ushort objectID;                                                                                                                                 \
    byte active;

#if RETRO_USE_PLUS
#define RSDK_ENTITY                                                                                                                                  \
    Vector2 position;                                                                                                                                \
    Vector2 scale;                                                                                                                                   \
    Vector2 velocity;                                                                                                                                \
    Vector2 updateRange;                                                                                                                             \
    int angle;                                                                                                                                       \
    int alpha;                                                                                                                                       \
    int rotation;                                                                                                                                    \
    int groundVel;                                                                                                                                   \
    int depth;                                                                                                                                       \
    ushort group;                                                                                                                                    \
    ushort objectID;                                                                                                                                 \
    bool32 inBounds;                                                                                                                                 \
    bool32 isPermanent;                                                                                                                              \
    bool32 tileCollisions;                                                                                                                           \
    bool32 interaction;                                                                                                                              \
    bool32 onGround;                                                                                                                                 \
    byte active;                                                                                                                                     \
    byte filter;                                                                                                                                     \
    byte direction;                                                                                                                                  \
    byte drawOrder;                                                                                                                                  \
    byte collisionLayers;                                                                                                                            \
    byte collisionPlane;                                                                                                                             \
    byte collisionMode;                                                                                                                              \
    byte drawFX;                                                                                                                                     \
    byte inkEffect;                                                                                                                                  \
    byte visible;                                                                                                                                    \
    byte activeScreens;
#else
#define RSDK_ENTITY                                                                                                                                  \
    Vector2 position;                                                                                                                                \
    Vector2 scale;                                                                                                                                   \
    Vector2 velocity;                                                                                                                                \
    Vector2 updateRange;                                                                                                                             \
    int angle;                                                                                                                                       \
    int alpha;                                                                                                                                       \
    int rotation;                                                                                                                                    \
    int groundVel;                                                                                                                                   \
    int depth;                                                                                                                                       \
    ushort group;                                                                                                                                    \
    ushort objectID;                                                                                                                                 \
    bool32 inBounds;                                                                                                                                 \
    bool32 isPermanent;                                                                                                                              \
    bool32 tileCollisions;                                                                                                                           \
    bool32 interaction;                                                                                                                              \
    bool32 onGround;                                                                                                                                 \
    byte active;                                                                                                                                     \
    byte direction;                                                                                                                                  \
    byte drawOrder;                                                                                                                                  \
    byte collisionLayers;                                                                                                                            \
    byte collisionPlane;                                                                                                                             \
    byte collisionMode;                                                                                                                              \
    byte drawFX;                                                                                                                                     \
    byte inkEffect;                                                                                                                                  \
    byte visible;                                                                                                                                    \
    byte activeScreens;
#endif

#define ENTITY_SIZE (sizeof(Entity) + (0x100 * sizeof(void*)))

#if RETRO_USE_PLUS
typedef struct {
    int platform;
    int language;
    int region;
} SKUInfo;

typedef struct {
    int field_0;
    int field_4;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    int field_24;
    int field_28;
    int field_2C;
} UnknownInfo;
#endif

typedef struct {
    char engineInfo[0x40];
    char gameSubname[0x100];
    char version[0x10];
#if !RETRO_USE_PLUS
    byte platform;
    byte language;
    byte region;
#endif
} EngineInfo;

typedef struct {
    Entity *entity;
    void *listData;
    void *listCategory;
    int timeCounter;
    int currentDrawGroup;
    int currentScreenID;
    ushort listPos;
    ushort entitySlot;
    ushort createSlot;
    ushort classCount;
    bool32 inEditor;
    bool32 effectGizmo;
    bool32 debugMode;
    bool32 useGlobalScripts;
    bool32 timeEnabled;
    byte activeCategory;
    byte categoryCount;
    byte state;
#if RETRO_USE_PLUS
    byte filter;
#endif
    byte milliseconds;
    byte seconds;
    byte minutes;
} SceneInfo;

typedef struct {
    bool32 down;
    bool32 press;
    int keyMap;
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
} ControllerState;

typedef struct {
    InputState keyUp;
    InputState keyDown;
    InputState keyLeft;
    InputState keyRight;
    InputState keyStick;
    float deadzone;
    float hDelta;
    float vDelta;
} AnalogState;

#if RETRO_USE_PLUS
typedef struct {
    InputState key1;
    InputState key2;
    float unknown1;
    float unknown2;
} TriggerState;
#endif

typedef struct {
    float x[0x10];
    float y[0x10];
    bool32 down[0x10];
    byte count;
} TouchMouseData;

typedef struct {
    // ushort *frameBuffer;
    ushort frameBuffer[SCREEN_XMAX * SCREEN_YSIZE];
    Vector2 position;
    int width;
    int height;
    int centerX;
    int centerY;
    int pitch;
    int clipBound_X1;
    int clipBound_Y1;
    int clipBound_X2;
    int clipBound_Y2;
    int waterDrawPos;
} ScreenInfo;

typedef struct {
    void *functionPtrs;
#if RETRO_USE_PLUS
    void *APIPtrs;
    SKUInfo *currentSKU;
#endif
    EngineInfo *engineInfo;
    SceneInfo *sceneInfo;
    ControllerState *controller;
    AnalogState *stickL;
#if RETRO_USE_PLUS
    AnalogState *stickR;
    TriggerState *triggerL;
    TriggerState *triggerR;
#endif
    TouchMouseData *touchMouse;
#if RETRO_USE_PLUS
    UnknownInfo *unknown;
#endif
    ScreenInfo *screenInfo;
} GameInfo;

typedef struct {
    int values[4][4];
} Matrix;

typedef struct {
    ushort *text;
    ushort textLength;
    ushort length;
} TextInfo;

typedef struct {
    short left;
    short top;
    short right;
    short bottom;
} Hitbox;

typedef struct {
    ushort sprX;
    ushort sprY;
    ushort width;
    ushort height;
    short pivotX;
    short pivotY;
    ushort delay;
    short id;
    byte sheetID;
    byte hitboxCnt;
    Hitbox hitboxes[8];
} SpriteFrame;

typedef struct {
    SpriteFrame *framePtrs;
    int frameID;
    short animationID;
    short prevAnimationID;
    short animationSpeed;
    short animationTimer;
    short frameDelay;
    short frameCount;
    byte loopIndex;
    byte rotationFlag;
} Animator;

typedef struct {
    int tilePos;
    int parallaxFactor;
    int scrollSpeed;
    int scrollPos;
    byte deform;
    byte unknown;
} ScrollInfo;

typedef struct {
    Vector2 position;
    Vector2 deform;
} ScanlineInfo;

typedef struct {
    byte behaviour;
    byte drawLayer[4];
    byte widthShift;
    byte heightShift;
    ushort width;
    ushort height;
    Vector2 position;
    int parallaxFactor;
    int scrollSpeed;
    int scrollPos;
    int deformationOffset;
    int deformationOffsetW;
    int deformationData[0x400];
    int deformationDataW[0x400];
    void (*scanlineCallback)(ScanlineInfo *);
    ushort scrollInfoCount;
    ScrollInfo scrollInfo[0x100];
    uint name[4];
    ushort *layout;
    byte *lineScroll;
} TileLayer;

typedef struct {
    byte statID;
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
    ALIGN_LEFT,
    ALIGN_RIGHT,
    ALIGN_CENTER,
} Alignments;

typedef enum {
    SEVERITY_NONE,
    SEVERITY_WARN,
    SEVERITY_ERROR,
    SEVERITY_FATAL,
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

typedef enum {
    SECRET_RICKYMODE = 0,
    SECRET_SUPERDASH = 1,
} GameCheats;

typedef enum {
    GROUP_ALL     = 0,
    GROUP_CUSTOM1 = TYPE_COUNT,
    GROUP_CUSTOM2,
    GROUP_CUSTOM3,
    GROUP_CUSTOM4,
} TypeGroups;

typedef enum { STATUS_CONTINUE = 100, STATUS_OK = 200, STATUS_FORBIDDEN = 403, STATUS_NOTFOUND = 404, STATUS_ERROR = 500 } StatusCodes;

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

#endif /* GAMELINK_H */
