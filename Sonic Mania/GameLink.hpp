#ifndef GAMELINK_H
#define GAMELINK_H

// ================
// STANDARD LIBS
// ================
#include <iostream>
#include <cstddef>

// ================
// STANDARD TYPES
// ================
typedef unsigned char byte;
typedef signed char sbyte;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef long long int64;
typedef unsigned long long uint64;
typedef unsigned int bool32;

#define SCREEN_YSIZE    (240)
#define LAYER_COUNT     (8)
#define DRAWLAYER_COUNT (16)

struct Vector2 {
    int x;
    int y;
};

struct Entity {
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
    int field_3C;
    bool32 tileCollisions;
    bool32 interaction;
    bool32 onGround;
    byte active;
    byte filter;
    byte direction;
    byte drawOrder;
    byte collisionLayers;
    byte collisionPlane;
    byte collisionMode;
    byte drawFX;
    byte inkEffect;
    byte visible;
    byte activeScreens;
    byte field_57;
};

struct Object {
    short objectID;
    byte active;
    byte field_3;
};

struct SKUInfo {
    int platform;
    int language;
    int region;
};

struct SceneInfo {
    Entity *entity;
    void *listData;
    void *listCategory;
    int timeCounter;
    int currentDrawGroup;
    int currentScreenID;
    int listPos;
    int entitySlot;
    int createSlot;
    int classCount;
    bool32 inEditor;
    bool32 effectGizmo;
    bool32 debugMode;
    bool32 useGlobalScripts;
    bool32 timeEnabled;
    byte activeCategory;
    byte categoryCount;
    byte state;
    byte filter;
    byte milliseconds;
    byte seconds;
    byte minutes;
};

struct InputState {
    bool32 down;
    bool32 press;
    int keyMap;

    inline void setHeld()
    {
        press = !down;
        down  = true;
    }
    inline void setReleased()
    {
        press = false;
        down  = false;
    }
};

struct ControllerState {
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
};

struct AnalogState {
    InputState keyUp;
    InputState keyDown;
    InputState keyLeft;
    InputState keyRight;
    InputState keyStick;
    float unknown;
    float hDelta;
    float vDelta;
};

struct TriggerState {
    InputState key1;
    InputState key2;
    float unknown1;
    float unknown2;
};

struct TouchMouseData {
    float x[0x10];
    float y[0x10];
    bool32 down[0x10];
    byte count;
};

struct ScreenInfo {
    ushort *frameBuffer;
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
};

struct GameInfo {
    void *functionPtrs;
    void *userdataPtrs;
    char *gameName;
    SKUInfo *currentSKU;
    SceneInfo *sceneInfo;
    ControllerState *controller;
    AnalogState *stickL;
    AnalogState *stickR;
    TriggerState *triggerL;
    TriggerState *triggerR;
    TouchMouseData *touchMouse;
    void *inputCount;
    ScreenInfo *screenInfo;
};

struct Matrix {
    int values[4][4];
};

struct TextInfo {
    ushort* text;
    short textLength;
    short length;
};

struct Hitbox {
    short left;
    short top;
    short right;
    short bottom;
};

struct SpriteFrame {
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
};

struct EntityAnimationData {
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
};

struct ScrollInfo {
    int unknown;
    int parallaxFactor;
    int scrollSpeed;
    int scrollPos;
    bool32 deform;
};

struct ScanlineInfo {
    Vector2 position;
    Vector2 deform;
};

struct TileLayer {
    byte behaviour;
    byte drawLayer[4];
    byte widthShift;
    byte heightShift;
    byte field_7;
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
    char field_202E;
    char field_202F;
    ScrollInfo scrollInfo[0x100];
    uint name[4];
    ushort *layout;
    byte *lineScroll;
};

extern char textBuffer[0x400];

enum GamePlatforms {
    PLATFORM_WIN    = 0,
    PLATFORM_PS4    = 1,
    PLATFORM_XB1    = 2,
    PLATFORM_SWITCH = 3,
    PLATFORM_DEV    = 0xFF,
};

enum Scopes {
    SCOPE_NONE,
    SCOPE_GLOBAL,
    SCOPE_STAGE,
};

enum InkEffects {
    INK_NONE,
    INK_BLEND,
    INK_ALPHA,
    INK_ADD,
    INK_SUB,
    INK_LOOKUP,
    INK_MASKED,
    INK_UNMASKED,
};

enum DrawFX { FX_NONE = 0, FX_FLIP = 1, FX_ROTATE = 2, FX_SCALE = 4 };

enum FlipFlags { FLIP_NONE, FLIP_X, FLIP_Y, FLIP_XY };

enum Alignments {
    ALIGN_LEFT,
    ALIGN_RIGHT,
    ALIGN_CENTER,
};

enum SeverityModes {
    SEVERITY_NONE,
    SEVERITY_WARN,
    SEVERITY_ERROR,
    SEVERITY_FATAL,
};

enum AttributeTypes {
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
};

enum ActiveFlags {
    ACTIVE_NEVER,
    ACTIVE_ALWAYS,
    ACTIVE_NORMAL,
    ACTIVE_PAUSED,
    ACTIVE_BOUNDS,
    ACTIVE_XBOUNDS,
    ACTIVE_YBOUNDS,
    ACTIVE_RBOUNDS,
};

enum LayerTypes {
    LAYER_HSCROLL,
    LAYER_VSCROLL,
    LAYER_ROTOZOOM,
    LAYER_BASIC,
};

enum EngineStates {
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
    ENGINESTATE_ERRORMSG         = 0x0B,
    ENGINESTATE_ERRORMSG_FATAL   = 0x0C,
    ENGINESTATE_NULL             = 0x0D,
};

#endif /* GAMELINK_H */
