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
#define PLAYER_MAX (4)
#else
#define PLAYER_MAX (2)
#endif

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
    bool32 isPermament;
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
    bool32 isPermament;                                                                                                                              \
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
    bool32 isPermament;                                                                                                                              \
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

#define ENTITY_SIZE (sizeof(Entity) + 0x400)

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
    byte field_30;
} UnknownInfo;
#else
typedef struct {
    char gameName[32];
    char gameSubname[32];
    char version[16];
    char field_50;
    char field_51;
    char field_52;
    ushort objectCount;
    char field_55;
    char field_56;
    char field_57;
    char field_58;
    char field_59;
    char field_5A;
    char field_5B;
    char field_5C;
    char field_5D;
    char field_5E;
    char field_5F;
    char field_60;
    char field_61;
    char field_62;
    char field_63;
    char field_64;
    char field_65;
    char field_66;
    char field_67;
    char field_68;
    char field_69;
    char field_6A;
    char field_6B;
    char field_6C;
    char field_6D;
    char field_6E;
    char field_6F;
    char field_70;
    char field_71;
    char field_72;
    char field_73;
    char field_74;
    char field_75;
    char field_76;
    char field_77;
    char field_78;
    char field_79;
    char field_7A;
    char field_7B;
    char field_7C;
    char field_7D;
    char field_7E;
    char field_7F;
    char field_80;
    char field_81;
    char field_82;
    char field_83;
    char field_84;
    char field_85;
    char field_86;
    char field_87;
    char field_88;
    char field_89;
    char field_8A;
    char field_8B;
    char field_8C;
    char field_8D;
    char field_8E;
    char field_8F;
    char field_90;
    char field_91;
    char field_92;
    char field_93;
    char field_94;
    char field_95;
    char field_96;
    char field_97;
    char field_98;
    char field_99;
    char field_9A;
    char field_9B;
    char field_9C;
    char field_9D;
    char field_9E;
    char field_9F;
    char field_A0;
    char field_A1;
    char field_A2;
    char field_A3;
    char field_A4;
    char field_A5;
    char field_A6;
    char field_A7;
    char field_A8;
    char field_A9;
    char field_AA;
    char field_AB;
    char field_AC;
    char field_AD;
    char field_AE;
    char field_AF;
    char field_B0;
    char field_B1;
    char field_B2;
    char field_B3;
    char field_B4;
    char field_B5;
    char field_B6;
    char field_B7;
    char field_B8;
    char field_B9;
    char field_BA;
    char field_BB;
    char field_BC;
    char field_BD;
    char field_BE;
    char field_BF;
    char field_C0;
    char field_C1;
    char field_C2;
    char field_C3;
    char field_C4;
    char field_C5;
    char field_C6;
    char field_C7;
    char field_C8;
    char field_C9;
    char field_CA;
    char field_CB;
    char field_CC;
    char field_CD;
    char field_CE;
    char field_CF;
    char field_D0;
    char field_D1;
    char field_D2;
    char field_D3;
    char field_D4;
    char field_D5;
    char field_D6;
    char field_D7;
    char field_D8;
    char field_D9;
    char field_DA;
    char field_DB;
    char field_DC;
    char field_DD;
    char field_DE;
    char field_DF;
    char field_E0;
    char field_E1;
    char field_E2;
    char field_E3;
    char field_E4;
    char field_E5;
    char field_E6;
    char field_E7;
    char field_E8;
    char field_E9;
    char field_EA;
    char field_EB;
    char field_EC;
    char field_ED;
    char field_EE;
    char field_EF;
    char field_F0;
    char field_F1;
    char field_F2;
    char field_F3;
    char field_F4;
    char field_F5;
    char field_F6;
    char field_F7;
    char field_F8;
    char field_F9;
    char field_FA;
    char field_FB;
    char field_FC;
    char field_FD;
    char field_FE;
    char field_FF;
    char field_100;
    char field_101;
    char field_102;
    char field_103;
    char field_104;
    char field_105;
    char field_106;
    char field_107;
    char field_108;
    char field_109;
    char field_10A;
    char field_10B;
    char field_10C;
    char field_10D;
    char field_10E;
    char field_10F;
    char field_110;
    char field_111;
    char field_112;
    char field_113;
    char field_114;
    char field_115;
    char field_116;
    char field_117;
    char field_118;
    char field_119;
    char field_11A;
    char field_11B;
    char field_11C;
    char field_11D;
    char field_11E;
    char field_11F;
    char field_120;
    char field_121;
    char field_122;
    char field_123;
    char field_124;
    char field_125;
    char field_126;
    char field_127;
    char field_128;
    char field_129;
    char field_12A;
    char field_12B;
    char field_12C;
    char field_12D;
    char field_12E;
    char field_12F;
    char field_130;
    char field_131;
    char field_132;
    char field_133;
    char field_134;
    char field_135;
    char field_136;
    char field_137;
    char field_138;
    char field_139;
    char field_13A;
    char field_13B;
    char field_13C;
    char field_13D;
    char field_13E;
    char field_13F;
    char field_140;
    char field_141;
    char field_142;
    char field_143;
    char playerBoundActiveB;
    char field_145;
    char field_146;
    char field_147;
    char field_148;
    char field_149;
    char field_14A;
    char field_14B;
    char field_14C;
    char field_14D;
    char field_14E;
    char field_14F;
    byte platformID;
    byte language;
    byte region;
    char field_153;
    char field_154;
    char field_155;
    char field_156;
    char field_157;
    char field_158;
    char field_159;
    char field_15A;
    char field_15B;
    char field_15C;
    char field_15D;
    char field_15E;
    char field_15F;
    char field_160;
    char field_161;
    char field_162;
    char field_163;
    char field_164;
    char field_165;
    char field_166;
    char field_167;
    char field_168;
    char field_169;
    char field_16A;
    char field_16B;
    char field_16C;
    char field_16D;
    char field_16E;
    char field_16F;
    char field_170;
    char field_171;
    char field_172;
    char field_173;
    char field_174;
    char field_175;
    char field_176;
    char field_177;
    char field_178;
    char field_179;
    char field_17A;
    char field_17B;
    char field_17C;
    char field_17D;
    char field_17E;
    char field_17F;
} EngineInfo;
#endif

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
    void *userdataPtrs;
    char *gameName;
    SKUInfo *currentSKU;
#endif
#if !RETRO_USE_PLUS
    EngineInfo *engineInfo;
#endif
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
} AnimationData;

typedef struct {
    int deadzone;
    int parallaxFactor;
    int scrollSpeed;
    int scrollPos;
    bool32 deform;
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
    ACTIVE_NEVER,
    ACTIVE_ALWAYS,
    ACTIVE_NORMAL,
    ACTIVE_PAUSED,
    ACTIVE_BOUNDS,
    ACTIVE_XBOUNDS,
    ACTIVE_YBOUNDS,
    ACTIVE_RBOUNDS,
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

#define CallFunction(func)                                                                                                                           \
    if (func)                                                                                                                                        \
        func();

#endif /* GAMELINK_H */
