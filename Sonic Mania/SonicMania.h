#ifndef SONIC_MANIA_H
#define SONIC_MANIA_H

#define VER_100 0 // 1.00 (initial console release)
#define VER_103 3 // 1.03 (pc release/ console patch)
#define VER_105 5 // 1.04/1.05
#define VER_106 6 // 1.06 (steam denuvo removal update)
#define VER_107 7 // 1.07 (EGS/Origin releases)

#define RETRO_GAMEVER VER_106

#define RETRO_USE_PLUS (RETRO_GAMEVER >= VER_105)
#define RETRO_USE_EGS  (RETRO_GAMEVER == VER_107)

#define RETRO_USE_MOD_LOADER (1)

// used to manage standalone (RSDKv5.exe & Game.dll) and combined (Game.exe) modes
#ifndef RETRO_STANDALONE
#define RETRO_STANDALONE (1)
#endif

// slightly misleading, this controls adding touch controls to players
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
    int32 collapseDelay;                                                                                                                             \
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
    void (*options2)(void);                                                                                                                          \
    void (*options3)(void);                                                                                                                          \
    void (*failCB)(void);                                                                                                                            \
    void (*options5)(void);                                                                                                                          \
    void (*options6)(void);                                                                                                                          \
    bool32 (*options7)(void);                                                                                                                        \
    bool32 (*options8)(void);                                                                                                                        \
    int32 timer;                                                                                                                                     \
    Vector2 posUnknown2;                                                                                                                             \
    Entity *parent;                                                                                                                                  \
    Vector2 touchPosStart;                                                                                                                           \
    Vector2 touchPosEnd;                                                                                                                             \
    bool32 touchPressed;                                                                                                                             \
    Vector2 touchPos1[4];                                                                                                                            \
    Vector2 touchPos2[4];                                                                                                                            \
    void (*touchPosCallbacks[4])(void);                                                                                                              \
    int32 touchPosCount;                                                                                                                             \
    int32 touchCountUnknown;                                                                                                                         \
    bool32 flag;                                                                                                                                     \
    bool32 disabled;

#define MANIA_CUTSCENE_BASE                                                                                                                          \
    RSDK_ENTITY                                                                                                                                      \
    Vector2 size;                                                                                                                                    \
    bool32 activated;                                                                                                                                \
    int32 field_64;                                                                                                                                  \
    int32 field_68;                                                                                                                                  \
    int32 field_6C;                                                                                                                                  \
    int32 field_70;                                                                                                                                  \
    int32 field_74;                                                                                                                                  \
    int32 field_78;                                                                                                                                  \
    Hitbox hitbox;

#include "GameVariables.h"
#include "GameObjects.h"

extern SceneInfo *RSDK_sceneInfo;
extern EngineInfo *RSDK_info;
#if RETRO_USE_PLUS
extern SKUInfo *RSDK_sku;
#endif
extern ControllerState *RSDK_controller;
extern AnalogState *RSDK_stickL;
#if RETRO_USE_PLUS
extern AnalogState *RSDK_stickR;
extern TriggerState *RSDK_triggerL;
extern TriggerState *RSDK_triggerR;
#endif
extern TouchMouseData *RSDK_touchMouse;
#if RETRO_USE_PLUS
extern UnknownInfo *RSDK_unknown;
#endif
extern ScreenInfo *RSDK_screens;

static const char *PlayerNames[] = { "Invalid", "Sonic", "Tails", "Knuckles", "Mighty", "Ray" };
static const char *ActNames[]    = { "Act 1", "Act 2", "Act 3" };
static const char *ZoneNames[]   = { "GHZ", "CPZ", "SPZ", "FBZ", "PGZ", "SSZ", "HCZ", "MSZ", "OOZ", "LRZ", "MMZ", "TMZ", "ERZ", "AIZ" };
static const char *ModeNames[]   = { "Mania", "Encore" };
static const char *AchNames[]    = { "ACH_GOLD_MEDAL", "ACH_SILVER_MEDAL", "ACH_EMERALDS", "ACH_GAME_CLEARED", "ACH_STARPOST", "ACH_SIGNPOST",
                                  "ACH_GHZ",        "ACH_CPZ",          "ACH_SPZ",      "ACH_FBZ",          "ACH_PGZ",      "ACH_SSZ",
                                  "ACH_HCZ",        "ACH_MSZ",          "ACH_OOZ",      "ACH_LRZ",          "ACH_MMZ",      "ACH_TMZ" };
// clang-format off
static const char *LeaderboardNames[] = {
    // Mania Mode
    "GHZ1_S_MANIA", "GHZ1_T_MANIA", "GHZ1_K_MANIA",  "GHZ1_M_MANIA", "GHZ1_R_MANIA",
    "GHZ2_S_MANIA", "GHZ2_T_MANIA", "GHZ2_K_MANIA",  "GHZ2_M_MANIA", "GHZ2_R_MANIA", 
    
    "CPZ1_S_MANIA", "CPZ1_T_MANIA", "CPZ1_K_MANIA",  "CPZ1_M_MANIA", "CPZ1_R_MANIA",
    "CPZ2_S_MANIA", "CPZ2_T_MANIA", "CPZ2_K_MANIA",  "CPZ2_M_MANIA", "CPZ2_R_MANIA", 
    
    "SPZ1_S_MANIA", "SPZ1_T_MANIA", "SPZ1_K_MANIA",  "SPZ1_M_MANIA", "SPZ1_R_MANIA",
    "SPZ2_S_MANIA", "SPZ2_T_MANIA", "SPZ2_K_MANIA",  "SPZ2_M_MANIA", "SPZ2_R_MANIA", 
    
    "FBZ1_S_MANIA", "FBZ1_T_MANIA", "FBZ1_K_MANIA",  "FBZ1_M_MANIA", "FBZ1_R_MANIA",
    "FBZ2_S_MANIA", "FBZ2_T_MANIA", "FBZ2_K_MANIA",  "FBZ2_M_MANIA", "FBZ2_R_MANIA", 
    
    "PGZ1_S_MANIA", "PGZ1_T_MANIA", "PGZ1_K_MANIA",  "PGZ1_M_MANIA", "PGZ1_R_MANIA",
    "PGZ2_S_MANIA", "PGZ2_T_MANIA", "PGZ2_K_MANIA",  "PGZ2_M_MANIA", "PGZ2_R_MANIA", 
    
    "SSZ1_S_MANIA", "SSZ1_T_MANIA", "SSZ1_K_MANIA",  "SSZ1_M_MANIA", "SSZ1_R_MANIA",
    "SSZ2_S_MANIA", "SSZ2_T_MANIA", "SSZ2_K_MANIA",  "SSZ2_M_MANIA", "SSZ2_R_MANIA", 
    
    "HCZ1_S_MANIA", "HCZ1_T_MANIA", "HCZ1_K_MANIA",  "HCZ1_M_MANIA", "HCZ1_R_MANIA",
    "HCZ2_S_MANIA", "HCZ2_T_MANIA", "HCZ2_K_MANIA",  "HCZ2_M_MANIA", "HCZ2_R_MANIA", 
    
    "NULL", "NULL", "MSZ1_K_MANIA", "NULL", "NULL",
    "MSZ2_S_MANIA", "MSZ2_T_MANIA", "MSZ2_K_MANIA",  "MSZ2_M_MANIA", "MSZ2_R_MANIA", 
    
    "OOZ1_S_MANIA", "OOZ1_T_MANIA", "OOZ1_K_MANIA",  "OOZ1_M_MANIA", "OOZ1_R_MANIA",
    "OOZ2_S_MANIA", "OOZ2_T_MANIA", "OOZ2_K_MANIA",  "OOZ2_M_MANIA", "OOZ2_R_MANIA", 
    
    "LRZ1_S_MANIA", "LRZ1_T_MANIA", "LRZ1_K_MANIA",  "LRZ1_M_MANIA", "LRZ1_R_MANIA",
    "LRZ2_S_MANIA", "LRZ2_T_MANIA", "LRZ2_K_MANIA",  "LRZ2_M_MANIA", "LRZ2_R_MANIA", 
    
    "MMZ1_S_MANIA", "MMZ1_T_MANIA", "MMZ1_K_MANIA",  "MMZ1_M_MANIA", "MMZ1_R_MANIA",
    "MMZ2_S_MANIA", "MMZ2_T_MANIA", "MMZ2_K_MANIA",  "MMZ2_M_MANIA", "MMZ2_R_MANIA", 
    
    "TMZ1_S_MANIA", "TMZ1_T_MANIA", "TMZ1_K_MANIA",  "TMZ1_M_MANIA", "TMZ1_R_MANIA",
    "TMZ2_S_MANIA", "TMZ2_T_MANIA", "TMZ2_K_MANIA",  "TMZ2_M_MANIA", "TMZ2_R_MANIA", 
    
    // Encore Mode
    "GHZ1_S_ENCORE", "GHZ1_T_ENCORE", "GHZ1_K_ENCORE",  "GHZ1_M_ENCORE", "GHZ1_R_ENCORE",
    "GHZ2_S_ENCORE", "GHZ2_T_ENCORE", "GHZ2_K_ENCORE",  "GHZ2_M_ENCORE", "GHZ2_R_ENCORE", 
    
    "CPZ1_S_ENCORE", "CPZ1_T_ENCORE", "CPZ1_K_ENCORE",  "CPZ1_M_ENCORE", "CPZ1_R_ENCORE",
    "CPZ2_S_ENCORE", "CPZ2_T_ENCORE", "CPZ2_K_ENCORE",  "CPZ2_M_ENCORE", "CPZ2_R_ENCORE", 
    
    "SPZ1_S_ENCORE", "SPZ1_T_ENCORE", "SPZ1_K_ENCORE",  "SPZ1_M_ENCORE", "SPZ1_R_ENCORE",
    "SPZ2_S_ENCORE", "SPZ2_T_ENCORE", "SPZ2_K_ENCORE",  "SPZ2_M_ENCORE", "SPZ2_R_ENCORE", 
    
    "FBZ1_S_ENCORE", "FBZ1_T_ENCORE", "FBZ1_K_ENCORE",  "FBZ1_M_ENCORE", "FBZ1_R_ENCORE",
    "FBZ2_S_ENCORE", "FBZ2_T_ENCORE", "FBZ2_K_ENCORE",  "FBZ2_M_ENCORE", "FBZ2_R_ENCORE", 
    
    "PGZ1_S_ENCORE", "PGZ1_T_ENCORE", "PGZ1_K_ENCORE",  "PGZ1_M_ENCORE", "PGZ1_R_ENCORE",
    "PGZ2_S_ENCORE", "PGZ2_T_ENCORE", "PGZ2_K_ENCORE",  "PGZ2_M_ENCORE", "PGZ2_R_ENCORE", 
    
    "SSZ1_S_ENCORE", "SSZ1_T_ENCORE", "SSZ1_K_ENCORE",  "SSZ1_M_ENCORE", "SSZ1_R_ENCORE",
    "SSZ2_S_ENCORE", "SSZ2_T_ENCORE", "SSZ2_K_ENCORE",  "SSZ2_M_ENCORE", "SSZ2_R_ENCORE", 
    
    "HCZ1_S_ENCORE", "HCZ1_T_ENCORE", "HCZ1_K_ENCORE",  "HCZ1_M_ENCORE", "HCZ1_R_ENCORE",
    "HCZ2_S_ENCORE", "HCZ2_T_ENCORE", "HCZ2_K_ENCORE",  "HCZ2_M_ENCORE", "HCZ2_R_ENCORE", 
    
    "MSZ1_S_ENCORE", "MSZ1_T_ENCORE", "MSZ1_K_ENCORE",  "MSZ1_M_ENCORE", "MSZ1_R_ENCORE",
    "MSZ2_S_ENCORE", "MSZ2_T_ENCORE", "MSZ2_K_ENCORE",  "MSZ2_M_ENCORE", "MSZ2_R_ENCORE", 
    
    "OOZ1_S_ENCORE", "OOZ1_T_ENCORE", "OOZ1_K_ENCORE",  "OOZ1_M_ENCORE", "OOZ1_R_ENCORE",
    "OOZ2_S_ENCORE", "OOZ2_T_ENCORE", "OOZ2_K_ENCORE",  "OOZ2_M_ENCORE", "OOZ2_R_ENCORE", 
    
    "LRZ1_S_ENCORE", "LRZ1_T_ENCORE", "LRZ1_K_ENCORE",  "LRZ1_M_ENCORE", "LRZ1_R_ENCORE",
    "LRZ2_S_ENCORE", "LRZ2_T_ENCORE", "LRZ2_K_ENCORE",  "LRZ2_M_ENCORE", "LRZ2_R_ENCORE", 
    
    "MMZ1_S_ENCORE", "MMZ1_T_ENCORE", "MMZ1_K_ENCORE",  "MMZ1_M_ENCORE", "MMZ1_R_ENCORE",
    "MMZ2_S_ENCORE", "MMZ2_T_ENCORE", "MMZ2_K_ENCORE",  "MMZ2_M_ENCORE", "MMZ2_R_ENCORE", 
    
    "TMZ1_S_ENCORE", "TMZ1_T_ENCORE", "TMZ1_K_ENCORE",  "TMZ1_M_ENCORE", "TMZ1_R_ENCORE",
    "TMZ2_S_ENCORE", "TMZ2_T_ENCORE", "TMZ2_K_ENCORE",  "TMZ2_M_ENCORE", "TMZ2_R_ENCORE"
};

static const char *LeaderboardNames2[]     = { 
    // Mania Mode
    "PlusGHZ1SonicMania", "PlusGHZ1TailsMania", "PlusGHZ1KnucklesMania", "PlusGHZ1MightyMania", "PlusGHZ1RayMania",
    "PlusGHZ2SonicMania", "PlusGHZ2TailsMania", "PlusGHZ2KnucklesMania", "PlusGHZ2MightyMania", "PlusGHZ2RayMania",
    
    "PlusCPZ1SonicMania", "PlusCPZ1TailsMania", "PlusCPZ1KnucklesMania", "PlusCPZ1MightyMania", "PlusCPZ1RayMania",
    "PlusCPZ2SonicMania", "PlusCPZ2TailsMania", "PlusCPZ2KnucklesMania", "PlusCPZ2MightyMania", "PlusCPZ2RayMania",

    "PlusSPZ1SonicMania", "PlusSPZ1TailsMania", "PlusSPZ1KnucklesMania", "PlusSPZ1MightyMania", "PlusSPZ1RayMania",
    "PlusSPZ2SonicMania", "PlusSPZ2TailsMania", "PlusSPZ2KnucklesMania", "PlusSPZ2MightyMania", "PlusSPZ2RayMania",

    "PlusFBZ1SonicMania", "PlusFBZ1TailsMania", "PlusFBZ1KnucklesMania", "PlusFBZ1MightyMania", "PlusFBZ1RayMania",
    "PlusFBZ2SonicMania", "PlusFBZ2TailsMania", "PlusFBZ2KnucklesMania", "PlusFBZ2MightyMania", "PlusFBZ2RayMania",

    "PlusPGZ1SonicMania", "PlusPGZ1TailsMania", "PlusPGZ1KnucklesMania", "PlusPGZ1MightyMania", "PlusPGZ1RayMania",
    "PlusPGZ2SonicMania", "PlusPGZ2TailsMania", "PlusPGZ2KnucklesMania", "PlusPGZ2MightyMania", "PlusPGZ2RayMania",

    "PlusSSZ1SonicMania", "PlusSSZ1TailsMania", "PlusSSZ1KnucklesMania", "PlusSSZ1MightyMania", "PlusSSZ1RayMania",
    "PlusSSZ2SonicMania", "PlusSSZ2TailsMania", "PlusSSZ2KnucklesMania", "PlusSSZ2MightyMania", "PlusSSZ2RayMania",

    "PlusHCZ1SonicMania", "PlusHCZ1TailsMania", "PlusHCZ1KnucklesMania", "PlusHCZ1MightyMania", "PlusHCZ1RayMania",
    "PlusHCZ2SonicMania", "PlusHCZ2TailsMania", "PlusHCZ2KnucklesMania", "PlusHCZ2MightyMania", "PlusHCZ2RayMania",

    "NULL", "NULL", "PlusMSZ1KnucklesMania", "NULL", "NULL",
    "PlusMSZ2SonicMania", "PlusMSZ2TailsMania", "PlusMSZ2KnucklesMania", "PlusMSZ2MightyMania", "PlusMSZ2RayMania",

    "PlusOOZ1SonicMania", "PlusOOZ1TailsMania", "PlusOOZ1KnucklesMania", "PlusOOZ1MightyMania", "PlusOOZ1RayMania",
    "PlusOOZ2SonicMania", "PlusOOZ2TailsMania", "PlusOOZ2KnucklesMania", "PlusOOZ2MightyMania", "PlusOOZ2RayMania",

    "PlusLRZ1SonicMania", "PlusLRZ1TailsMania", "PlusLRZ1KnucklesMania", "PlusLRZ1MightyMania", "PlusLRZ1RayMania",
    "PlusLRZ2SonicMania", "PlusLRZ2TailsMania", "PlusLRZ2KnucklesMania", "PlusLRZ2MightyMania", "PlusLRZ2RayMania",

    "PlusMMZ1SonicMania", "PlusMMZ1TailsMania", "PlusMMZ1KnucklesMania", "PlusMMZ1MightyMania", "PlusMMZ1RayMania",
    "PlusMMZ2SonicMania", "PlusMMZ2TailsMania", "PlusMMZ2KnucklesMania", "PlusMMZ2MightyMania", "PlusMMZ2RayMania",

    "PlusTMZ1SonicMania", "PlusTMZ1TailsMania", "PlusTMZ1KnucklesMania", "PlusTMZ1MightyMania", "PlusTMZ1RayMania",
    "PlusTMZ2SonicMania", "PlusTMZ2TailsMania", "PlusTMZ2KnucklesMania", "PlusTMZ2MightyMania", "PlusTMZ2RayMania",
    
    // Encore Mode
    "PlusGHZ1SonicEncore", "PlusGHZ1TailsEncore", "PlusGHZ1KnucklesEncore", "PlusGHZ1MightyEncore", "PlusGHZ1RayEncore",
    "PlusGHZ2SonicEncore", "PlusGHZ2TailsEncore", "PlusGHZ2KnucklesEncore", "PlusGHZ2MightyEncore", "PlusGHZ2RayEncore",
    
    "PlusCPZ1SonicEncore", "PlusCPZ1TailsEncore", "PlusCPZ1KnucklesEncore", "PlusCPZ1MightyEncore", "PlusCPZ1RayEncore",
    "PlusCPZ2SonicEncore", "PlusCPZ2TailsEncore", "PlusCPZ2KnucklesEncore", "PlusCPZ2MightyEncore", "PlusCPZ2RayEncore",

    "PlusSPZ1SonicEncore", "PlusSPZ1TailsEncore", "PlusSPZ1KnucklesEncore", "PlusSPZ1MightyEncore", "PlusSPZ1RayEncore",
    "PlusSPZ2SonicEncore", "PlusSPZ2TailsEncore", "PlusSPZ2KnucklesEncore", "PlusSPZ2MightyEncore", "PlusSPZ2RayEncore",

    "PlusFBZ1SonicEncore", "PlusFBZ1TailsEncore", "PlusFBZ1KnucklesEncore", "PlusFBZ1MightyEncore", "PlusFBZ1RayEncore",
    "PlusFBZ2SonicEncore", "PlusFBZ2TailsEncore", "PlusFBZ2KnucklesEncore", "PlusFBZ2MightyEncore", "PlusFBZ2RayEncore",

    "PlusPGZ1SonicEncore", "PlusPGZ1TailsEncore", "PlusPGZ1KnucklesEncore", "PlusPGZ1MightyEncore", "PlusPGZ1RayEncore",
    "PlusPGZ2SonicEncore", "PlusPGZ2TailsEncore", "PlusPGZ2KnucklesEncore", "PlusPGZ2MightyEncore", "PlusPGZ2RayEncore",

    "PlusSSZ1SonicEncore", "PlusSSZ1TailsEncore", "PlusSSZ1KnucklesEncore", "PlusSSZ1MightyEncore", "PlusSSZ1RayEncore",
    "PlusSSZ2SonicEncore", "PlusSSZ2TailsEncore", "PlusSSZ2KnucklesEncore", "PlusSSZ2MightyEncore", "PlusSSZ2RayEncore",

    "PlusHCZ1SonicEncore", "PlusHCZ1TailsEncore", "PlusHCZ1KnucklesEncore", "PlusHCZ1MightyEncore", "PlusHCZ1RayEncore",
    "PlusHCZ2SonicEncore", "PlusHCZ2TailsEncore", "PlusHCZ2KnucklesEncore", "PlusHCZ2MightyEncore", "PlusHCZ2RayEncore",

    "PlusMSZ1SonicEncore", "PlusMSZ1TailsEncore", "PlusMSZ1KnucklesEncore", "PlusMSZ1MightyEncore", "PlusMSZ1RayEncore",
    "PlusMSZ2SonicEncore", "PlusMSZ2TailsEncore", "PlusMSZ2KnucklesEncore", "PlusMSZ2MightyEncore", "PlusMSZ2RayEncore",

    "PlusOOZ1SonicEncore", "PlusOOZ1TailsEncore", "PlusOOZ1KnucklesEncore", "PlusOOZ1MightyEncore", "PlusOOZ1RayEncore",
    "PlusOOZ2SonicEncore", "PlusOOZ2TailsEncore", "PlusOOZ2KnucklesEncore", "PlusOOZ2MightyEncore", "PlusOOZ2RayEncore",

    "PlusLRZ1SonicEncore", "PlusLRZ1TailsEncore", "PlusLRZ1KnucklesEncore", "PlusLRZ1MightyEncore", "PlusLRZ1RayEncore",
    "PlusLRZ2SonicEncore", "PlusLRZ2TailsEncore", "PlusLRZ2KnucklesEncore", "PlusLRZ2MightyEncore", "PlusLRZ2RayEncore",

    "PlusMMZ1SonicEncore", "PlusMMZ1TailsEncore", "PlusMMZ1KnucklesEncore", "PlusMMZ1MightyEncore", "PlusMMZ1RayEncore",
    "PlusMMZ2SonicEncore", "PlusMMZ2TailsEncore", "PlusMMZ2KnucklesEncore", "PlusMMZ2MightyEncore", "PlusMMZ2RayEncore",

    "PlusTMZ1SonicEncore", "PlusTMZ1TailsEncore", "PlusTMZ1KnucklesEncore", "PlusTMZ1MightyEncore", "PlusTMZ1RayEncore",
    "PlusTMZ2SonicEncore", "PlusTMZ2TailsEncore", "PlusTMZ2KnucklesEncore", "PlusTMZ2MightyEncore", "PlusTMZ2RayEncore"
};
//clang-format on

#endif //!SONIC_MANIA_H
