#ifndef OBJ_MSZSETUP_H
#define OBJ_MSZSETUP_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 fadeTimer;
    uint16 aniTiles;
    TileLayer *bg;
    TABLE(int32 deformData[32], { -1, 0, 0, -1, -1, 0, 0, 0, 0, -1, -1, 0, 0, 0, 0, -1, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 });
    STATIC(int32 aniTilesTimerA, 16);
    STATIC(int32 aniTilesTimerB, 30);
    int32 aniTilesFrameA;
    int32 aniTilesFrameB;
    int32 aniTilesFrameC;
    bool32 flag;
    TABLE(int32 aniTilesSizeA[14], { 0, 1, 2, 3, 4, 5, 2, 3, 4, 5, 2, 1, 6, 7 });
    TABLE(int32 aniTilesDelayA[14], { 16, 2, 3, 3, 2, 2, 2, 2, 2, 3, 3, 3, 2, 3 });
    TABLE(int32 aniTilesDelayB[8], { 16, 2, 4, 5, 16, 2, 4, 5 });
    int32 field_138[512];
    int32 parallaxMult;
    int32 parallaxPos;
    bool32 hasAchievement;
    Entity *msz2Cutscene;
    int32 starPostID;
#if RETRO_USE_PLUS
    int32 chuggaVolume;
    int32 chuggaChannel;
    uint16 sfxLocoChugga;
#endif
} ObjectMSZSetup;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
} EntityMSZSetup;

// Object Struct
extern ObjectMSZSetup *MSZSetup;

// Standard Entity Events
void MSZSetup_Update(void);
void MSZSetup_LateUpdate(void);
void MSZSetup_StaticUpdate(void);
void MSZSetup_Draw(void);
void MSZSetup_Create(void* data);
void MSZSetup_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void MSZSetup_EditorDraw(void);
void MSZSetup_EditorLoad(void);
#endif
void MSZSetup_Serialize(void);

// Extra Entity Functions
void MSZSetup_Unknown2(void);
void MSZSetup_Unknown3(void);
void MSZSetup_Unknown4(int32 a1);

void MSZSetup_ManageFadeST(void);
void MSZSetup_ManageFadeK(void);
#if RETRO_USE_PLUS
void MSZSetup_ManageFadeE(void);
#endif

void MSZSetup_StageFinishCB_ST(void);
void MSZSetup_StageFinishCB_K(void);
#if RETRO_USE_PLUS
void MSZSetup_StageFinishCB_E(void);
#endif

void MSZSetup_ActivateMSZ2Cutscene(void);
void MSZSetup_GetAchievement(void);
void MSZSetup_HandleRestart(void);
void MSZSetup_SwitchPalettes(void);

void MSZSetup_Unknown9(void);
#if RETRO_USE_PLUS
void MSZSetup_Unknown10(void);
void MSZSetup_Unknown11(void);
void MSZSetup_Unknown12(void);
void MSZSetup_Unknown13(void);
void MSZSetup_Unknown14(void);
void MSZSetup_Unknown16(void);
void MSZSetup_Unknown17(void);
void MSZSetup_Unknown18(void);
void MSZSetup_Unknown19(void);
#endif
void MSZSetup_Unknown21(void);

void MSZSetup_Player_State_Pilot(void);
void MSZSetup_Player_State_PostCrashJumpIn(void);

#endif //!OBJ_MSZSETUP_H
