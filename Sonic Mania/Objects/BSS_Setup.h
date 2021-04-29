#ifndef OBJ_BSS_SETUP_H
#define OBJ_BSS_SETUP_H

#include "../SonicMania.h"

#define BSS_PLAYFIELD_W (0x20)
#define BSS_PLAYFIELD_H (0x20)

// Object Class
typedef struct {
    RSDK_OBJECT
    byte flags[4];
    int sphereCount;
    int pinkSphereCount;
    int rings;
    int ringPan;
    int ringCount;
    int field_1C;
    ushort bgLayer;
    ushort globeLayer;
    ushort frustrum1Layer;
    ushort frustrum2Layer;
    ushort playFieldLayer;
    ushort ringCountLayer;
    ushort globeMappings;
    int globeFrameTable[0xF];        //= { 0, 1, 2, 3, 4, 5, 6, 7, 6, 5, 4, 3, 2, 1, 0 };
    int globeDirTableL[0xF];        //= { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1 };
    int globeDirTableR[0xF];        //= { 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 };
    int screenYTable[0x70]; //= { 280, 270, 260, 251, 243, 235, 228, 221, 215, 208, 202, 197, 191, 185, 180, 175, 170, 165, 160, 155, 151, 147, 143,
                            // 139, 135, 131, 127, 124, 121, 117, 114, 111, 108, 105, 103, 100, 97, 95, 92, 90, 88, 86, 83, 81, 79, 77, 76, 74, 72,
                            // 70, 69, 67, 66, 64, 63, 62, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 47, 46, 45, 45, 44, 44, 43, 43, 42,
                            // 42, 41, 40, 40, 40, 40, 40, 40, 40, 40, 39, 39, 39, 39, 39, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38,
                            // 38, 38, 38 };
    int divisorTable[0x70]; //= { 4096, 4032, 3968, 3904, 3840, 3776, 3712, 3648, 3584, 3520, 3456, 3392, 3328, 3264, 3200, 3136, 3072, 2995, 2920,
                            // 2847, 2775, 2706, 2639, 2572, 2508, 2446, 2384, 2324, 2266, 2210, 2154, 2100, 2048, 2012, 1976, 1940, 1906, 1872, 1838,
                            // 1806, 1774, 1742, 1711, 1680, 1650, 1621, 1592, 1564, 1536, 1509, 1482, 1455, 1429, 1404, 1379, 1354, 1330, 1307, 1283,
                            // 1260, 1238, 1216, 1194, 1173, 1152, 1134, 1116, 1099, 1082, 1065, 1048, 1032, 1016, 1000, 985, 969, 954, 939, 925, 910,
                            // 896, 892, 888, 884, 880, 875, 871, 867, 863, 859, 855, 851, 848, 844, 840, 836, 832, 830, 828, 826, 824, 822, 820, 818,
                            // 816, 814, 812, 810, 808, 806, 804, 802 };
    int xMultiplierTable[0x70]; //= { 134, 131, 128, 125, 123, 121, 119, 117, 115, 114, 112, 111, 109, 108, 106, 104, 104, 102, 100, 98, 97, 96, 94,
                                // 93, 92, 90, 89, 88, 86, 84, 83, 82, 80, 80, 79, 78, 77, 76, 74, 73, 72, 71, 70, 70, 68, 68, 67, 66, 65, 64, 63, 62,
                                // 61, 60, 60, 59, 58, 57, 57, 56, 55, 54, 53, 53, 52, 51, 51, 50, 50, 49, 48, 48, 47, 47, 46, 46, 45, 45, 44, 44, 44,
                                // 43, 43, 43, 42, 42, 42, 41, 41, 41, 41, 40, 40, 40, 40, 39, 39, 39, 39, 39, 38, 38, 38, 38, 38, 37, 37, 37, 37, 37,
                                // 36, 36 };
    int frameTable[0x80]; //= { 31, 31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 30, 29, 29, 29, 29, 29, 28, 28, 28, 28, 27, 27, 27, 26, 26, 26, 26,
                           // 25, 25, 25, 24, 24, 24, 24, 23, 23, 23, 23, 22, 22, 22, 22, 21, 21, 21, 21, 20, 20, 20, 20, 19, 19, 19, 19, 18, 18, 18,
                           // 18, 17, 17, 17, 17, 16, 16, 16, 15, 15, 14, 14, 14, 13, 13, 13, 12, 12, 12, 11, 11, 10, 10, 10, 10, 9, 9, 9, 9, 8, 8, 8,
                           // 8, 7, 7, 7, 7, 6, 6, 6, 6, 5, 5, 5, 5, 4, 4, 3, 3, 2, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    Vector2 offsetTable[0x100];
    int offsetRadiusTable[0x100];
    int frustrumOffsetCount[2];
    int frustrumOffset[2];
    int field_1434;
    ushort playField[BSS_PLAYFIELD_W * BSS_PLAYFIELD_H];
    ushort playField2[BSS_PLAYFIELD_W * BSS_PLAYFIELD_H];
    ushort playField3[BSS_PLAYFIELD_W * BSS_PLAYFIELD_H];
    ushort sfxBlueSphere;
    ushort sfxSSExit;
    ushort sfxBumper;
    ushort sfxSpring;
    ushort sfxRing;
    ushort sfxLoseRings;
    ushort sfxSSJettison;
    ushort sfxEmerald;
    ushort sfxEvent;
    ushort sfxMedal;
    ushort sfxMedalCaught;
    ushort sfxTeleport;
} ObjectBSS_Setup;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int spinTimer;
    int speedupTimer;
    int speedupInterval;
    int timer;
    int spinState;
    int palettePage;
    int field_74;
    int xMultiplier;
    int divisor;
    int maxSpeed;
    int globeSpeed;
    bool32 unknownFlag;
    int globeSpeedInc;
    bool32 bumperFlag;
    int globeTimer;
    int paletteLine;
    int offsetDir;
    int field_A0;
    Vector2 offset;
    Vector2 playerPos;
    Vector2 lastSpherePos;
    int field_BC;
    bool32 ringLoopFlag;
    int paletteID;
    int pauseFlag;
    Animator globeSpinData;
    Animator shadowData;
} EntityBSS_Setup;

// Object Struct
extern ObjectBSS_Setup *BSS_Setup;

// Standard Entity Events
void BSS_Setup_Update(void);
void BSS_Setup_LateUpdate(void);
void BSS_Setup_StaticUpdate(void);
void BSS_Setup_Draw(void);
void BSS_Setup_Create(void *data);
void BSS_Setup_StageLoad(void);
void BSS_Setup_EditorDraw(void);
void BSS_Setup_EditorLoad(void);
void BSS_Setup_Serialize(void);

// Extra Entity Functions
int BSS_Setup_ReloadScene(void);
void BSS_Setup_SetupPalette(void);
void BSS_Setup_SetupCollectables(void);
void BSS_Setup_CollectRing(void);
void BSS_Setup_GetStartupInfo(void);
void BSS_Setup_Finished(void);
void BSS_Setup_HandleSteppedObjects(void);
void BSS_Setup_HandleCollectableMovement(void);
void BSS_Setup_State_FinishWalk(void);
void BSS_Setup_State_PinkSphereWarp(void);
void BSS_Setup_State_Exit(void);
void BSS_Setup_State_HandleStage(void);
void BSS_Setup_State_SpinLeft(void);
void BSS_Setup_State_SpinRight(void);
void BSS_Setup_State_Unknown23(void);
bool32 BSS_Setup_Unknown(int x, int y);
void BSS_Setup_Unknown12(void);
void BSS_Setup_Unknown13(void);
bool32 BSS_Setup_Unknown2(byte x, byte y);
bool32 BSS_Setup_Unknown3(byte x, byte y);
bool32 BSS_Setup_Unknown4(byte x, byte y);
bool32 BSS_Setup_Unknown5(byte x, byte y);
bool32 BSS_Setup_Unknown6(byte x, byte y);
void BSS_Setup_ProcessChain(void);

#endif //! OBJ_BSS_SETUP_H
