#ifndef OBJ_MSZSETUP_H
#define OBJ_MSZSETUP_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int fadeTimer;
    ushort aniTiles;
    TileLayer *bg;
    int deformData[32]; //= { -1, 0, 0, -1, -1, 0, 0, 0, 0, -1, -1, 0, 0, 0, 0, -1, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int aniTilesTimerA; //= 16;
    int aniTilesTimerB; //= 30;
    int aniTilesFrameA;
    int aniTilesFrameB;
    int aniTilesFrameC;
    bool32 flag;
    int aniTilesSizeA[14]; //= { 0, 1, 2, 3, 4, 5, 2, 3, 4, 5, 2, 1, 6, 7 };
    int aniTilesDelayA[14]; //= { 16, 2, 3, 3, 2, 2, 2, 2, 2, 3, 3, 3, 2, 3 };
    int aniTilesDelayB[8];  //= { 16, 2, 4, 5, 16, 2, 4, 5 };
    int field_138[512];
    int parallaxMult;
    int field_93C;
    bool32 hasAchievement;
    Entity *msz2Cutscene;
    int field_948;
#if RETRO_USE_PLUS
    int chuggaVolume;
    int chuggaChannel;
    ushort sfxLocoChugga;
#endif
} ObjectMSZSetup;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int timer;
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
void MSZSetup_EditorDraw(void);
void MSZSetup_EditorLoad(void);
void MSZSetup_Serialize(void);

// Extra Entity Functions
void MSZSetup_Unknown2(void);
void MSZSetup_Unknown4(int a1);

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
#endif

void MSZSetup_PlayerState_Pilot(void);

#endif //!OBJ_MSZSETUP_H
