#ifndef SONIC_MANIA_H
#define SONIC_MANIA_H

// =====================================================================
// RSDK Project: Sonic Mania (Plus)
// Decompiled by: Rubberduckycooly & RMGRich
// =====================================================================

#define VER_100 0 // 1.00 (initial console release)
#define VER_103 3 // 1.03 (pc release/ console patch)
#define VER_105 5 // 1.04/1.05
#define VER_106 6 // 1.06 (steam denuvo removal update)
#define VER_107 7 // 1.07 (EGS/Origin releases)

#ifdef MANIA_PREPLUS
#define RETRO_GAMEVER VER_103
#else
#define RETRO_GAMEVER VER_106
#endif

#define RETRO_USE_PLUS (RETRO_GAMEVER >= VER_105)
#define RETRO_USE_EGS  (RETRO_GAMEVER == VER_107)

// Controls whether EditorLoad & EditorDraw should be included in the final product or not
// This is a copy of what the original game likely had, as the original game does not include EditorLoad or EditorDraw funcs for any objects
#define RETRO_INCLUDE_EDITOR (1)

#ifndef RETRO_USE_MOD_LOADER
#define RETRO_USE_MOD_LOADER (1)
#endif

#define RETRO_MOD_LOADER_VER (1)

// used to manage standalone (RSDKv5.exe & Game.dll) and combined (Game.exe) modes
#ifndef RETRO_STANDALONE
#define RETRO_STANDALONE (1)
#endif

// slightly misleading, this controls adding touch controls to the game
#define RETRO_USE_TOUCH_CONTROLS (1)

#include "GameLink.h"

#ifdef _MSC_VER
#define DLLExport       __declspec(dllexport)
#define setAlignment(x) __declspec(align(x))
#else
#define DLLExport
#define setAlignment(x)
#endif

// "Base" Classes
#define MANIA_PLATFORM_BASE                                                                                                                          \
    RSDK_ENTITY                                                                                                                                      \
    StateMachine(state);                                                                                                                             \
    StateMachine(stateCollide);                                                                                                                      \
    int32 type;                                                                                                                                      \
    Vector2 amplitude;                                                                                                                               \
    int32 speed;                                                                                                                                     \
    bool32 hasTension;                                                                                                                               \
    int8 frameID;                                                                                                                                    \
    uint8 collision;                                                                                                                                 \
    Vector2 tileOrigin;                                                                                                                              \
    Vector2 centerPos;                                                                                                                               \
    Vector2 drawPos;                                                                                                                                 \
    Vector2 collisionOffset;                                                                                                                         \
    int32 stood;                                                                                                                                     \
    int32 timer;                                                                                                                             \
    int32 stoodAngle;                                                                                                                                \
    uint8 stoodPlayers;                                                                                                                              \
    uint8 pushPlayersL;                                                                                                                              \
    uint8 pushPlayersR;                                                                                                                              \
    Hitbox hitbox;                                                                                                                                   \
    Animator animator;                                                                                                                               \
    int32 childCount;

#define MANIA_BUTTON_BASE                                                                                                                            \
    RSDK_ENTITY                                                                                                                                      \
    int32 type;                                                                                                                                      \
    bool32 walkOnto;                                                                                                                                 \
    uint8 tag;                                                                                                                                       \
    bool32 down;                                                                                                                                     \
    bool32 toggled;                                                                                                                                  \
    bool32 currentlyActive;                                                                                                                          \
    bool32 activated;                                                                                                                                \
    bool32 wasActivated;                                                                                                                             \
    int32 pressPos;

#define MANIA_UI_ITEM_BASE                                                                                                                           \
    RSDK_ENTITY                                                                                                                                      \
    StateMachine(state);                                                                                                                             \
    void (*processButtonCB)(void);                                                                                                                   \
    bool32 (*touchCB)(void);                                                                                                                         \
    void (*actionCB)(void);                                                                                                                          \
    void (*selectedCB)(void);                                                                                                                        \
    void (*failCB)(void);                                                                                                                            \
    void (*buttonEnterCB)(void);                                                                                                                     \
    void (*buttonLeaveCB)(void);                                                                                                                     \
    bool32 (*checkButtonEnterCB)(void);                                                                                                              \
    bool32 (*checkSelectedCB)(void);                                                                                                                 \
    int32 timer;                                                                                                                                     \
    Vector2 startPos;                                                                                                                                \
    Entity *parent;                                                                                                                                  \
    Vector2 touchPosSizeS;                                                                                                                           \
    Vector2 touchPosOffsetS;                                                                                                                             \
    bool32 touchPressed;                                                                                                                             \
    Vector2 touchPosSizeM[4];   /*size of the touchPos: in 16-bit shifted format*/                                                                    \
    Vector2 touchPosOffsetM[4]; /*offset of the touchPos: 0,0 is entity pos, negative is left/up, positive is right/down*/                            \
    void (*touchPosCallbacks[4])(void);                                                                                                              \
    int32 touchPosCount;                                                                                                                             \
    int32 touchPosID;                                                                                                                                \
    bool32 isSelected;                                                                                                                               \
    bool32 disabled;

#define MANIA_CUTSCENE_BASE                                                                                                                          \
    RSDK_ENTITY                                                                                                                                      \
    Vector2 size;                                                                                                                                    \
    bool32 activated;                                                                                                                                \
    Animator animator; /* unused, afaik */                                                                                                           \
    Hitbox hitbox;

#include "GameVariables.h"
#include "GameObjects.h"

extern RSDKSceneInfo *SceneInfo;
extern RSDKGameInfo *GameInfo;
#if RETRO_USE_PLUS
extern RSDKSKUInfo *SKU;
#endif
extern RSDKControllerState *ControllerInfo;
extern RSDKAnalogState *AnalogStickInfoL;
#if RETRO_USE_PLUS
extern RSDKAnalogState *AnalogStickInfoR;
extern RSDKTriggerState *TriggerInfoL;
extern RSDKTriggerState *TriggerInfoR;
#endif
extern RSDKTouchInfo *TouchInfo;
#if RETRO_USE_PLUS
extern RSDKUnknownInfo *UnknownInfo;
#endif
extern RSDKScreenInfo *ScreenInfo;

#endif //!SONIC_MANIA_H
