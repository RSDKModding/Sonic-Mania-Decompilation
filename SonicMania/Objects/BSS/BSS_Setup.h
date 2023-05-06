#ifndef OBJ_BSS_SETUP_H
#define OBJ_BSS_SETUP_H

#include "Game.h"

#define BSS_PLAYFIELD_W (0x20)
#define BSS_PLAYFIELD_H (0x20)

// Object Class
struct ObjectBSS_Setup {
    RSDK_OBJECT
    uint8 randomNumbers[4]; // used to calculate the map (& colours) for Random BSS mode
    int32 sphereCount;
    int32 pinkSphereCount;
    int32 rings;
    int32 ringPan;
    int32 ringCount;
    int32 ringID; // updated in BSS_Collected, but aside from that it goes pretty much unused it seems
    uint16 bgLayer;
    uint16 globeLayer;
    uint16 frustum1Layer;
    uint16 frustum2Layer;
    uint16 playFieldLayer;
    uint16 ringCountLayer;
    uint16 globeFrames;
    TABLE(int32 globeFrameTable[0xF], { 0, 1, 2, 3, 4, 5, 6, 7, 6, 5, 4, 3, 2, 1, 0 });
    TABLE(int32 globeDirTableL[0xF], { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1 });
    TABLE(int32 globeDirTableR[0xF], { 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 });
    TABLE(int32 screenYTable[0x70],
          { 280, 270, 260, 251, 243, 235, 228, 221, 215, 208, 202, 197, 191, 185, 180, 175, 170, 165, 160, 155, 151, 147, 143,
            139, 135, 131, 127, 124, 121, 117, 114, 111, 108, 105, 103, 100, 97,  95,  92,  90,  88,  86,  83,  81,  79,  77,
            76,  74,  72,  70,  69,  67,  66,  64,  63,  62,  60,  59,  58,  57,  56,  55,  54,  53,  52,  51,  50,  49,  48,
            47,  47,  46,  45,  45,  44,  44,  43,  43,  42,  42,  41,  40,  40,  40,  40,  40,  40,  40,  40,  39,  39,  39,
            39,  39,  38,  38,  38,  38,  38,  38,  38,  38,  38,  38,  38,  38,  38,  38,  38,  38,  38,  38 });
    TABLE(int32 divisorTable[0x70],
          { 4096, 4032, 3968, 3904, 3840, 3776, 3712, 3648, 3584, 3520, 3456, 3392, 3328, 3264, 3200, 3136, 3072, 2995, 2920, 2847, 2775, 2706, 2639,
            2572, 2508, 2446, 2384, 2324, 2266, 2210, 2154, 2100, 2048, 2012, 1976, 1940, 1906, 1872, 1838, 1806, 1774, 1742, 1711, 1680, 1650, 1621,
            1592, 1564, 1536, 1509, 1482, 1455, 1429, 1404, 1379, 1354, 1330, 1307, 1283, 1260, 1238, 1216, 1194, 1173, 1152, 1134, 1116, 1099, 1082,
            1065, 1048, 1032, 1016, 1000, 985,  969,  954,  939,  925,  910,  896,  892,  888,  884,  880,  875,  871,  867,  863,  859,  855,  851,
            848,  844,  840,  836,  832,  830,  828,  826,  824,  822,  820,  818,  816,  814,  812,  810,  808,  806,  804,  802 });
    TABLE(int32 xMultiplierTable[0x70],
          { 134, 131, 128, 125, 123, 121, 119, 117, 115, 114, 112, 111, 109, 108, 106, 104, 104, 102, 100, 98, 97, 96, 94, 93, 92, 90, 89, 88,
            86,  84,  83,  82,  80,  80,  79,  78,  77,  76,  74,  73,  72,  71,  70,  70,  68,  68,  67,  66, 65, 64, 63, 62, 61, 60, 60, 59,
            58,  57,  57,  56,  55,  54,  53,  53,  52,  51,  51,  50,  50,  49,  48,  48,  47,  47,  46,  46, 45, 45, 44, 44, 44, 43, 43, 43,
            42,  42,  42,  41,  41,  41,  41,  40,  40,  40,  40,  39,  39,  39,  39,  39,  38,  38,  38,  38, 38, 37, 37, 37, 37, 37, 36, 36 });
    TABLE(int32 frameTable[0x80],
          { 31, 31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 30, 29, 29, 29, 29, 29, 28, 28, 28, 28, 27, 27, 27, 26, 26, 26, 26, 25, 25, 25,
            24, 24, 24, 24, 23, 23, 23, 23, 22, 22, 22, 22, 21, 21, 21, 21, 20, 20, 20, 20, 19, 19, 19, 19, 18, 18, 18, 18, 17, 17, 17, 17,
            16, 16, 16, 15, 15, 14, 14, 14, 13, 13, 13, 12, 12, 12, 11, 11, 10, 10, 10, 10, 9,  9,  9,  9,  8,  8,  8,  8,  7,  7,  7,  7,
            6,  6,  6,  6,  5,  5,  5,  5,  4,  4,  3,  3,  2,  2,  1,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 });
    Vector2 offsetTable[0x100];
    int32 offsetRadiusTable[0x100];
    int32 frustumCount[2];
    int32 frustumOffset[2];
    int32 unused1;
    uint16 playField[0x400];            // Active Spheres & Collectables (0x400 == 0x20 * 0x20 == BSS_PLAYFIELD_W * BSS_PLAYFIELD_H)
    uint16 sphereChainTable[0x400];     // Currently chained spheres     (0x400 == 0x20 * 0x20 == BSS_PLAYFIELD_W * BSS_PLAYFIELD_H)
    uint16 sphereCollectedTable[0x400]; // Spheres to turn into rings    (0x400 == 0x20 * 0x20 == BSS_PLAYFIELD_W * BSS_PLAYFIELD_H)
    uint16 sfxBlueSphere;
    uint16 sfxSSExit;
    uint16 sfxBumper;
    uint16 sfxSpring;
    uint16 sfxRing;
    uint16 sfxLoseRings;
    uint16 sfxSSJettison;
    uint16 sfxEmerald;
    uint16 sfxEvent;
    uint16 sfxMedal;
    uint16 sfxMedalCaught;
    uint16 sfxTeleport;
};

// Entity Class
struct EntityBSS_Setup {
    RSDK_ENTITY
    StateMachine(state);
    int32 spinTimer;
    int32 speedupTimer;
    int32 speedupInterval;
    int32 timer;
    int32 spinState;
    int32 palettePage;
    int32 unused1;
    int32 xMultiplier;
    int32 divisor;
    int32 speedupLevel;
    int32 globeSpeed;
    bool32 playerWasBumped;
    int32 globeSpeedInc;
    bool32 disableBumpers;
    int32 globeTimer;
    int32 paletteLine;
    int32 offsetDir;
    int32 unused2;
    Vector2 offset;
    Vector2 playerPos;
    Vector2 lastSpherePos;
    int32 unused3;
    bool32 completedRingLoop;
    int32 paletteID;
    int32 stopMovement;
    Animator globeSpinAnimator;
    Animator shadowAnimator;
};

// Object Struct
extern ObjectBSS_Setup *BSS_Setup;

// Standard Entity Events
void BSS_Setup_Update(void);
void BSS_Setup_LateUpdate(void);
void BSS_Setup_StaticUpdate(void);
void BSS_Setup_Draw(void);
void BSS_Setup_Create(void *data);
void BSS_Setup_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void BSS_Setup_EditorDraw(void);
void BSS_Setup_EditorLoad(void);
#endif
void BSS_Setup_Serialize(void);

// Extra Entity Functions
int32 BSS_Setup_GetStageID(void);
void BSS_Setup_SetupPalette(void);
void BSS_Setup_SetupFrustum(void);
void BSS_Setup_CollectRing(void);
void BSS_Setup_GetStartupInfo(void);
void BSS_Setup_State_GlobeJettison(void);
void BSS_Setup_HandleSteppedObjects(void);
void BSS_Setup_HandleCollectableMovement(void);
void BSS_Setup_State_GlobeEmerald(void);
void BSS_Setup_State_StartGlobeTeleport(void);
void BSS_Setup_State_GlobeExit(void);
void BSS_Setup_State_GlobeMoveZ(void);
void BSS_Setup_State_GlobeTurnLeft(void);
void BSS_Setup_State_GlobeTurnRight(void);
void BSS_Setup_State_FinishGlobeTeleport(void);
bool32 BSS_Setup_CheckSphereValid(int32 x, int32 y);
void BSS_Setup_LaunchSpheres(void);
void BSS_Setup_SetupFinishSequence(void);
bool32 BSS_Setup_ScanSphereChain_Up(uint8 x, uint8 y);
bool32 BSS_Setup_ScanSphereChain_Down(uint8 x, uint8 y);
bool32 BSS_Setup_ScanSphereChain_Left(uint8 x, uint8 y);
bool32 BSS_Setup_ScanSphereChain_Right(uint8 x, uint8 y);
bool32 BSS_Setup_GetChainedSphereCount(uint8 x, uint8 y);
void BSS_Setup_ProcessChain(void);

#endif //! OBJ_BSS_SETUP_H
