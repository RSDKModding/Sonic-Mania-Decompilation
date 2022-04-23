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

#if RETRO_USE_PLUS
typedef enum { DLC_PLUS } GameDLC;
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

static const char *PlayerNames[] = { "Invalid", "Sonic", "Tails", "Knuckles", "Mighty", "Ray" };
static const char *ActNames[]    = { "Act 1", "Act 2", "Act 3", "" };
static const char *ZoneNames[]   = { "GHZ", "CPZ", "SPZ", "FBZ", "PGZ", "SSZ", "HCZ", "MSZ", "OOZ", "LRZ", "MMZ", "TMZ", "ERZ", "AIZ" };
static const char *ModeNames[]   = { "Mania", "Encore" };

static LeaderboardID maniaLeaderboardInfo[] = { { 0, 1, 71, "PlusGHZ1SonicMania", "GHZ1_S_MANIA" },
                                 { 1, 2, 72, "PlusGHZ1TailsMania", "GHZ1_T_MANIA" },
                                 { 2, 3, 73, "PlusGHZ1KnucklesMania", "GHZ1_K_MANIA" },
                                 { 3, 4, 74, "PlusGHZ1MightyMania", "GHZ1_M_MANIA" },
                                 { 4, 5, 75, "PlusGHZ1RayMania", "GHZ1_R_MANIA" },
                                 { 5, 6, 76, "PlusGHZ2SonicMania", "GHZ2_S_MANIA" },
                                 { 6, 7, 77, "PlusGHZ2TailsMania", "GHZ2_T_MANIA" },
                                 { 7, 8, 78, "PlusGHZ2KnucklesMania", "GHZ2_K_MANIA" },
                                 { 8, 9, 79, "PlusGHZ2MightyMania", "GHZ2_M_MANIA" },
                                 { 9, 10, 80, "PlusGHZ2RayMania", "GHZ2_R_MANIA" },
                                 { 10, 11, 81, "PlusCPZ1SonicMania", "CPZ1_S_MANIA" },
                                 { 11, 12, 82, "PlusCPZ1TailsMania", "CPZ1_T_MANIA" },
                                 { 12, 13, 83, "PlusCPZ1KnucklesMania", "CPZ1_K_MANIA" },
                                 { 13, 14, 84, "PlusCPZ1MightyMania", "CPZ1_M_MANIA" },
                                 { 14, 15, 85, "PlusCPZ1RayMania", "CPZ1_R_MANIA" },
                                 { 15, 16, 86, "PlusCPZ2SonicMania", "CPZ2_S_MANIA" },
                                 { 16, 17, 87, "PlusCPZ2TailsMania", "CPZ2_T_MANIA" },
                                 { 17, 18, 88, "PlusCPZ2KnucklesMania", "CPZ2_K_MANIA" },
                                 { 18, 19, 89, "PlusCPZ2MightyMania", "CPZ2_M_MANIA" },
                                 { 19, 20, 90, "PlusCPZ2RayMania", "CPZ2_R_MANIA" },
                                 { 20, 21, 91, "PlusSPZ1SonicMania", "SPZ1_S_MANIA" },
                                 { 21, 22, 92, "PlusSPZ1TailsMania", "SPZ1_T_MANIA" },
                                 { 22, 23, 93, "PlusSPZ1KnucklesMania", "SPZ1_K_MANIA" },
                                 { 23, 24, 94, "PlusSPZ1MightyMania", "SPZ1_M_MANIA" },
                                 { 24, 25, 95, "PlusSPZ1RayMania", "SPZ1_R_MANIA" },
                                 { 25, 26, 96, "PlusSPZ2SonicMania", "SPZ2_S_MANIA" },
                                 { 26, 27, 97, "PlusSPZ2TailsMania", "SPZ2_T_MANIA" },
                                 { 27, 28, 98, "PlusSPZ2KnucklesMania", "SPZ2_K_MANIA" },
                                 { 28, 29, 99, "PlusSPZ2MightyMania", "SPZ2_M_MANIA" },
                                 { 29, 30, 100, "PlusSPZ2RayMania", "SPZ2_R_MANIA" },
                                 { 30, 31, 101, "PlusFBZ1SonicMania", "FBZ1_S_MANIA" },
                                 { 31, 32, 102, "PlusFBZ1TailsMania", "FBZ1_T_MANIA" },
                                 { 32, 33, 103, "PlusFBZ1KnucklesMania", "FBZ1_K_MANIA" },
                                 { 33, 34, 104, "PlusFBZ1MightyMania", "FBZ1_M_MANIA" },
                                 { 34, 35, 105, "PlusFBZ1RayMania", "FBZ1_R_MANIA" },
                                 { 35, 36, 106, "PlusFBZ2SonicMania", "FBZ2_S_MANIA" },
                                 { 36, 37, 107, "PlusFBZ2TailsMania", "FBZ2_T_MANIA" },
                                 { 37, 38, 108, "PlusFBZ2KnucklesMania", "FBZ2_K_MANIA" },
                                 { 38, 39, 109, "PlusFBZ2MightyMania", "FBZ2_M_MANIA" },
                                 { 39, 40, 110, "PlusFBZ2RayMania", "FBZ2_R_MANIA" },
                                 { 40, 41, 111, "PlusPGZ1SonicMania", "PGZ1_S_MANIA" },
                                 { 41, 42, 112, "PlusPGZ1TailsMania", "PGZ1_T_MANIA" },
                                 { 42, 43, 113, "PlusPGZ1KnucklesMania", "PGZ1_K_MANIA" },
                                 { 43, 44, 114, "PlusPGZ1MightyMania", "PGZ1_M_MANIA" },
                                 { 44, 45, 115, "PlusPGZ1RayMania", "PGZ1_R_MANIA" },
                                 { 45, 46, 116, "PlusPGZ2SonicMania", "PGZ2_S_MANIA" },
                                 { 46, 47, 117, "PlusPGZ2TailsMania", "PGZ2_T_MANIA" },
                                 { 47, 48, 118, "PlusPGZ2KnucklesMania", "PGZ2_K_MANIA" },
                                 { 48, 49, 119, "PlusPGZ2MightyMania", "PGZ2_M_MANIA" },
                                 { 49, 50, 120, "PlusPGZ2RayMania", "PGZ2_R_MANIA" },
                                 { 50, 51, 121, "PlusSSZ1SonicMania", "SSZ1_S_MANIA" },
                                 { 51, 52, 122, "PlusSSZ1TailsMania", "SSZ1_T_MANIA" },
                                 { 52, 53, 123, "PlusSSZ1KnucklesMania", "SSZ1_K_MANIA" },
                                 { 53, 54, 124, "PlusSSZ1MightyMania", "SSZ1_M_MANIA" },
                                 { 54, 55, 125, "PlusSSZ1RayMania", "SSZ1_R_MANIA" },
                                 { 55, 56, 126, "PlusSSZ2SonicMania", "SSZ2_S_MANIA" },
                                 { 56, 57, 127, "PlusSSZ2TailsMania", "SSZ2_T_MANIA" },
                                 { 57, 58, 128, "PlusSSZ2KnucklesMania", "SSZ2_K_MANIA" },
                                 { 58, 59, 129, "PlusSSZ2MightyMania", "SSZ2_M_MANIA" },
                                 { 59, 60, 130, "PlusSSZ2RayMania", "SSZ2_R_MANIA" },
                                 { 60, 61, 131, "PlusHCZ1SonicMania", "HCZ1_S_MANIA" },
                                 { 61, 62, 132, "PlusHCZ1TailsMania", "HCZ1_T_MANIA" },
                                 { 62, 63, 133, "PlusHCZ1KnucklesMania", "HCZ1_K_MANIA" },
                                 { 63, 64, 134, "PlusHCZ1MightyMania", "HCZ1_M_MANIA" },
                                 { 64, 65, 135, "PlusHCZ1RayMania", "HCZ1_R_MANIA" },
                                 { 65, 66, 136, "PlusHCZ2SonicMania", "HCZ2_S_MANIA" },
                                 { 66, 67, 137, "PlusHCZ2TailsMania", "HCZ2_T_MANIA" },
                                 { 67, 68, 138, "PlusHCZ2KnucklesMania", "HCZ2_K_MANIA" },
                                 { 68, 69, 139, "PlusHCZ2MightyMania", "HCZ2_M_MANIA" },
                                 { 69, 70, 140, "PlusHCZ2RayMania", "HCZ2_R_MANIA" },
                                 { -1, -1, -1, NULL, NULL },
                                 { -1, -1, -1, NULL, NULL },
                                 { 72, 71, 141, "PlusMSZ1KnucklesMania", "MSZ1_K_MANIA" },
                                 { -1, -1, -1, NULL, NULL },
                                 { -1, -1, -1, NULL, NULL },
                                 { 75, 72, 142, "PlusMSZ2SonicMania", "MSZ2_S_MANIA" },
                                 { 76, 73, 143, "PlusMSZ2TailsMania", "MSZ2_T_MANIA" },
                                 { 77, 74, 144, "PlusMSZ2KnucklesMania", "MSZ2_K_MANIA" },
                                 { 78, 75, 145, "PlusMSZ2MightyMania", "MSZ2_M_MANIA" },
                                 { 79, 76, 146, "PlusMSZ2RayMania", "MSZ2_R_MANIA" },
                                 { 80, 77, 147, "PlusOOZ1SonicMania", "OOZ1_S_MANIA" },
                                 { 81, 78, 148, "PlusOOZ1TailsMania", "OOZ1_T_MANIA" },
                                 { 82, 79, 149, "PlusOOZ1KnucklesMania", "OOZ1_K_MANIA" },
                                 { 83, 80, 150, "PlusOOZ1MightyMania", "OOZ1_M_MANIA" },
                                 { 84, 81, 151, "PlusOOZ1RayMania", "OOZ1_R_MANIA" },
                                 { 85, 82, 152, "PlusOOZ2SonicMania", "OOZ2_S_MANIA" },
                                 { 86, 83, 153, "PlusOOZ2TailsMania", "OOZ2_T_MANIA" },
                                 { 87, 84, 154, "PlusOOZ2KnucklesMania", "OOZ2_K_MANIA" },
                                 { 88, 85, 155, "PlusOOZ2MightyMania", "OOZ2_M_MANIA" },
                                 { 89, 86, 156, "PlusOOZ2RayMania", "OOZ2_R_MANIA" },
                                 { 90, 87, 157, "PlusLRZ1SonicMania", "LRZ1_S_MANIA" },
                                 { 91, 88, 158, "PlusLRZ1TailsMania", "LRZ1_T_MANIA" },
                                 { 92, 89, 159, "PlusLRZ1KnucklesMania", "LRZ1_K_MANIA" },
                                 { 93, 90, 160, "PlusLRZ1MightyMania", "LRZ1_M_MANIA" },
                                 { 94, 91, 161, "PlusLRZ1RayMania", "LRZ1_R_MANIA" },
                                 { 95, 92, 162, "PlusLRZ2SonicMania", "LRZ2_S_MANIA" },
                                 { 96, 93, 163, "PlusLRZ2TailsMania", "LRZ2_T_MANIA" },
                                 { 97, 94, 164, "PlusLRZ2KnucklesMania", "LRZ2_K_MANIA" },
                                 { 98, 95, 165, "PlusLRZ2MightyMania", "LRZ2_M_MANIA" },
                                 { 99, 96, 166, "PlusLRZ2RayMania", "LRZ2_R_MANIA" },
                                 { 100, 97, 167, "PlusMMZ1SonicMania", "MMZ1_S_MANIA" },
                                 { 101, 98, 168, "PlusMMZ1TailsMania", "MMZ1_T_MANIA" },
                                 { 102, 99, 169, "PlusMMZ1KnucklesMania", "MMZ1_K_MANIA" },
                                 { 103, 100, 170, "PlusMMZ1MightyMania", "MMZ1_M_MANIA" },
                                 { 104, 101, 171, "PlusMMZ1RayMania", "MMZ1_R_MANIA" },
                                 { 105, 102, 172, "PlusMMZ2SonicMania", "MMZ2_S_MANIA" },
                                 { 106, 103, 173, "PlusMMZ2TailsMania", "MMZ2_T_MANIA" },
                                 { 107, 104, 174, "PlusMMZ2KnucklesMania", "MMZ2_K_MANIA" },
                                 { 108, 105, 175, "PlusMMZ2MightyMania", "MMZ2_M_MANIA" },
                                 { 109, 106, 176, "PlusMMZ2RayMania", "MMZ2_R_MANIA" },
                                 { 110, 107, 177, "PlusTMZ1SonicMania", "TMZ1_S_MANIA" },
                                 { 111, 108, 178, "PlusTMZ1TailsMania", "TMZ1_T_MANIA" },
                                 { 112, 109, 179, "PlusTMZ1KnucklesMania", "TMZ1_K_MANIA" },
                                 { 113, 110, 180, "PlusTMZ1MightyMania", "TMZ1_M_MANIA" },
                                 { 114, 111, 181, "PlusTMZ1RayMania", "TMZ1_R_MANIA" },
                                 { 115, 112, 182, "PlusTMZ2SonicMania", "TMZ2_S_MANIA" },
                                 { 116, 113, 183, "PlusTMZ2TailsMania", "TMZ2_T_MANIA" },
                                 { 117, 114, 184, "PlusTMZ2KnucklesMania", "TMZ2_K_MANIA" },
                                 { 118, 115, 185, "PlusTMZ2MightyMania", "TMZ2_M_MANIA" },
                                 { 119, 116, 186, "PlusTMZ2RayMania", "TMZ2_R_MANIA" },
                                 { 120, 117, 187, "PlusGHZ1SonicEncore", "GHZ1_S_ENCORE" },
                                 { 121, 118, 188, "PlusGHZ1TailsEncore", "GHZ1_T_ENCORE" },
                                 { 122, 119, 189, "PlusGHZ1KnucklesEncore", "GHZ1_K_ENCORE" },
                                 { 123, 120, 190, "PlusGHZ1MightyEncore", "GHZ1_M_ENCORE" },
                                 { 124, 121, 191, "PlusGHZ1RayEncore", "GHZ1_R_ENCORE" },
                                 { 125, 122, 192, "PlusGHZ2SonicEncore", "GHZ2_S_ENCORE" },
                                 { 126, 123, 193, "PlusGHZ2TailsEncore", "GHZ2_T_ENCORE" },
                                 { 127, 124, 194, "PlusGHZ2KnucklesEncore", "GHZ2_K_ENCORE" },
                                 { 128, 125, 195, "PlusGHZ2MightyEncore", "GHZ2_M_ENCORE" },
                                 { 129, 126, 196, "PlusGHZ2RayEncore", "GHZ2_R_ENCORE" },
                                 { 130, 127, 197, "PlusCPZ1SonicEncore", "CPZ1_S_ENCORE" },
                                 { 131, 128, 198, "PlusCPZ1TailsEncore", "CPZ1_T_ENCORE" },
                                 { 132, 129, 199, "PlusCPZ1KnucklesEncore", "CPZ1_K_ENCORE" },
                                 { 133, 130, 200, "PlusCPZ1MightyEncore", "CPZ1_M_ENCORE" },
                                 { 134, 131, 201, "PlusCPZ1RayEncore", "CPZ1_R_ENCORE" },
                                 { 135, 132, 202, "PlusCPZ2SonicEncore", "CPZ2_S_ENCORE" },
                                 { 136, 133, 203, "PlusCPZ2TailsEncore", "CPZ2_T_ENCORE" },
                                 { 137, 134, 204, "PlusCPZ2KnucklesEncore", "CPZ2_K_ENCORE" },
                                 { 138, 135, 205, "PlusCPZ2MightyEncore", "CPZ2_M_ENCORE" },
                                 { 139, 136, 206, "PlusCPZ2RayEncore", "CPZ2_R_ENCORE" },
                                 { 140, 137, 207, "PlusSPZ1SonicEncore", "SPZ1_S_ENCORE" },
                                 { 141, 138, 208, "PlusSPZ1TailsEncore", "SPZ1_T_ENCORE" },
                                 { 142, 139, 209, "PlusSPZ1KnucklesEncore", "SPZ1_K_ENCORE" },
                                 { 143, 140, 210, "PlusSPZ1MightyEncore", "SPZ1_M_ENCORE" },
                                 { 144, 141, 211, "PlusSPZ1RayEncore", "SPZ1_R_ENCORE" },
                                 { 145, 142, 212, "PlusSPZ2SonicEncore", "SPZ2_S_ENCORE" },
                                 { 146, 143, 213, "PlusSPZ2TailsEncore", "SPZ2_T_ENCORE" },
                                 { 147, 144, 214, "PlusSPZ2KnucklesEncore", "SPZ2_K_ENCORE" },
                                 { 148, 145, 215, "PlusSPZ2MightyEncore", "SPZ2_M_ENCORE" },
                                 { 149, 146, 216, "PlusSPZ2RayEncore", "SPZ2_R_ENCORE" },
                                 { 150, 147, 217, "PlusFBZ1SonicEncore", "FBZ1_S_ENCORE" },
                                 { 151, 148, 218, "PlusFBZ1TailsEncore", "FBZ1_T_ENCORE" },
                                 { 152, 149, 219, "PlusFBZ1KnucklesEncore", "FBZ1_K_ENCORE" },
                                 { 153, 150, 220, "PlusFBZ1MightyEncore", "FBZ1_M_ENCORE" },
                                 { 154, 151, 221, "PlusFBZ1RayEncore", "FBZ1_R_ENCORE" },
                                 { 155, 152, 222, "PlusFBZ2SonicEncore", "FBZ2_S_ENCORE" },
                                 { 156, 153, 223, "PlusFBZ2TailsEncore", "FBZ2_T_ENCORE" },
                                 { 157, 154, 224, "PlusFBZ2KnucklesEncore", "FBZ2_K_ENCORE" },
                                 { 158, 155, 225, "PlusFBZ2MightyEncore", "FBZ2_M_ENCORE" },
                                 { 159, 156, 226, "PlusFBZ2RayEncore", "FBZ2_R_ENCORE" },
                                 { 160, 157, 227, "PlusPGZ1SonicEncore", "PGZ1_S_ENCORE" },
                                 { 161, 158, 228, "PlusPGZ1TailsEncore", "PGZ1_T_ENCORE" },
                                 { 162, 159, 229, "PlusPGZ1KnucklesEncore", "PGZ1_K_ENCORE" },
                                 { 163, 160, 230, "PlusPGZ1MightyEncore", "PGZ1_M_ENCORE" },
                                 { 164, 161, 231, "PlusPGZ1RayEncore", "PGZ1_R_ENCORE" },
                                 { 165, 162, 232, "PlusPGZ2SonicEncore", "PGZ2_S_ENCORE" },
                                 { 166, 163, 233, "PlusPGZ2TailsEncore", "PGZ2_T_ENCORE" },
                                 { 167, 164, 234, "PlusPGZ2KnucklesEncore", "PGZ2_K_ENCORE" },
                                 { 168, 165, 235, "PlusPGZ2MightyEncore", "PGZ2_M_ENCORE" },
                                 { 169, 166, 236, "PlusPGZ2RayEncore", "PGZ2_R_ENCORE" },
                                 { 170, 167, 237, "PlusSSZ1SonicEncore", "SSZ1_S_ENCORE" },
                                 { 171, 168, 238, "PlusSSZ1TailsEncore", "SSZ1_T_ENCORE" },
                                 { 172, 169, 239, "PlusSSZ1KnucklesEncore", "SSZ1_K_ENCORE" },
                                 { 173, 170, 240, "PlusSSZ1MightyEncore", "SSZ1_M_ENCORE" },
                                 { 174, 171, 241, "PlusSSZ1RayEncore", "SSZ1_R_ENCORE" },
                                 { 175, 172, 242, "PlusSSZ2SonicEncore", "SSZ2_S_ENCORE" },
                                 { 176, 173, 243, "PlusSSZ2TailsEncore", "SSZ2_T_ENCORE" },
                                 { 177, 174, 244, "PlusSSZ2KnucklesEncore", "SSZ2_K_ENCORE" },
                                 { 178, 175, 245, "PlusSSZ2MightyEncore", "SSZ2_M_ENCORE" },
                                 { 179, 176, 246, "PlusSSZ2RayEncore", "SSZ2_R_ENCORE" },
                                 { 180, 177, 247, "PlusHCZ1SonicEncore", "HCZ1_S_ENCORE" },
                                 { 181, 178, 248, "PlusHCZ1TailsEncore", "HCZ1_T_ENCORE" },
                                 { 182, 179, 249, "PlusHCZ1KnucklesEncore", "HCZ1_K_ENCORE" },
                                 { 183, 180, 250, "PlusHCZ1MightyEncore", "HCZ1_M_ENCORE" },
                                 { 184, 181, 251, "PlusHCZ1RayEncore", "HCZ1_R_ENCORE" },
                                 { 185, 182, 252, "PlusHCZ2SonicEncore", "HCZ2_S_ENCORE" },
                                 { 186, 183, 253, "PlusHCZ2TailsEncore", "HCZ2_T_ENCORE" },
                                 { 187, 184, 254, "PlusHCZ2KnucklesEncore", "HCZ2_K_ENCORE" },
                                 { 188, 185, 255, "PlusHCZ2MightyEncore", "HCZ2_M_ENCORE" },
                                 { 189, 186, 256, "PlusHCZ2RayEncore", "HCZ2_R_ENCORE" },
                                 { 190, 187, 257, "PlusMSZ1SonicEncore", "MSZ1_S_ENCORE" },
                                 { 191, 188, 258, "PlusMSZ1TailsEncore", "MSZ1_T_ENCORE" },
                                 { 192, 189, 259, "PlusMSZ1KnucklesEncore", "MSZ1_K_ENCORE" },
                                 { 193, 190, 260, "PlusMSZ1MightyEncore", "MSZ1_M_ENCORE" },
                                 { 194, 191, 261, "PlusMSZ1RayEncore", "MSZ1_R_ENCORE" },
                                 { 195, 192, 262, "PlusMSZ2SonicEncore", "MSZ2_S_ENCORE" },
                                 { 196, 193, 263, "PlusMSZ2TailsEncore", "MSZ2_T_ENCORE" },
                                 { 197, 194, 264, "PlusMSZ2KnucklesEncore", "MSZ2_K_ENCORE" },
                                 { 198, 195, 265, "PlusMSZ2MightyEncore", "MSZ2_M_ENCORE" },
                                 { 199, 196, 266, "PlusMSZ2RayEncore", "MSZ2_R_ENCORE" },
                                 { 200, 197, 267, "PlusOOZ1SonicEncore", "OOZ1_S_ENCORE" },
                                 { 201, 198, 268, "PlusOOZ1TailsEncore", "OOZ1_T_ENCORE" },
                                 { 202, 199, 269, "PlusOOZ1KnucklesEncore", "OOZ1_K_ENCORE" },
                                 { 203, 200, 270, "PlusOOZ1MightyEncore", "OOZ1_M_ENCORE" },
                                 { 204, 201, 271, "PlusOOZ1RayEncore", "OOZ1_R_ENCORE" },
                                 { 205, 202, 272, "PlusOOZ2SonicEncore", "OOZ2_S_ENCORE" },
                                 { 206, 203, 273, "PlusOOZ2TailsEncore", "OOZ2_T_ENCORE" },
                                 { 207, 204, 274, "PlusOOZ2KnucklesEncore", "OOZ2_K_ENCORE" },
                                 { 208, 205, 275, "PlusOOZ2MightyEncore", "OOZ2_M_ENCORE" },
                                 { 209, 206, 276, "PlusOOZ2RayEncore", "OOZ2_R_ENCORE" },
                                 { 210, 207, 277, "PlusLRZ1SonicEncore", "LRZ1_S_ENCORE" },
                                 { 211, 208, 278, "PlusLRZ1TailsEncore", "LRZ1_T_ENCORE" },
                                 { 212, 209, 279, "PlusLRZ1KnucklesEncore", "LRZ1_K_ENCORE" },
                                 { 213, 210, 280, "PlusLRZ1MightyEncore", "LRZ1_M_ENCORE" },
                                 { 214, 211, 281, "PlusLRZ1RayEncore", "LRZ1_R_ENCORE" },
                                 { 215, 212, 282, "PlusLRZ2SonicEncore", "LRZ2_S_ENCORE" },
                                 { 216, 213, 283, "PlusLRZ2TailsEncore", "LRZ2_T_ENCORE" },
                                 { 217, 214, 284, "PlusLRZ2KnucklesEncore", "LRZ2_K_ENCORE" },
                                 { 218, 215, 285, "PlusLRZ2MightyEncore", "LRZ2_M_ENCORE" },
                                 { 219, 216, 286, "PlusLRZ2RayEncore", "LRZ2_R_ENCORE" },
                                 { 220, 217, 287, "PlusMMZ1SonicEncore", "MMZ1_S_ENCORE" },
                                 { 221, 218, 288, "PlusMMZ1TailsEncore", "MMZ1_T_ENCORE" },
                                 { 222, 219, 289, "PlusMMZ1KnucklesEncore", "MMZ1_K_ENCORE" },
                                 { 223, 220, 290, "PlusMMZ1MightyEncore", "MMZ1_M_ENCORE" },
                                 { 224, 221, 291, "PlusMMZ1RayEncore", "MMZ1_R_ENCORE" },
                                 { 225, 222, 292, "PlusMMZ2SonicEncore", "MMZ2_S_ENCORE" },
                                 { 226, 223, 293, "PlusMMZ2TailsEncore", "MMZ2_T_ENCORE" },
                                 { 227, 224, 294, "PlusMMZ2KnucklesEncore", "MMZ2_K_ENCORE" },
                                 { 228, 225, 295, "PlusMMZ2MightyEncore", "MMZ2_M_ENCORE" },
                                 { 229, 226, 296, "PlusMMZ2RayEncore", "MMZ2_R_ENCORE" },
                                 { 230, 227, 297, "PlusTMZ1SonicEncore", "TMZ1_S_ENCORE" },
                                 { 231, 228, 298, "PlusTMZ1TailsEncore", "TMZ1_T_ENCORE" },
                                 { 232, 229, 299, "PlusTMZ1KnucklesEncore", "TMZ1_K_ENCORE" },
                                 { 233, 230, 300, "PlusTMZ1MightyEncore", "TMZ1_M_ENCORE" },
                                 { 234, 231, 301, "PlusTMZ1RayEncore", "TMZ1_R_ENCORE" },
                                 { 235, 232, 302, "PlusTMZ2SonicEncore", "TMZ2_S_ENCORE" },
                                 { 236, 233, 303, "PlusTMZ2TailsEncore", "TMZ2_T_ENCORE" },
                                 { 237, 234, 304, "PlusTMZ2KnucklesEncore", "TMZ2_K_ENCORE" },
                                 { 238, 235, 305, "PlusTMZ2MightyEncore", "TMZ2_M_ENCORE" },
                                 { 239, 236, 306, "PlusTMZ2RayEncore", "TMZ2_R_ENCORE" } };

#endif //!SONIC_MANIA_H
