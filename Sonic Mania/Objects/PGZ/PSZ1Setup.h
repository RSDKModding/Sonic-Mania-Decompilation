#ifndef OBJ_PSZ1SETUP_H
#define OBJ_PSZ1SETUP_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int aniTileDelays1[7];  //= { 64, 2, 1, 2, 2, 6, 2 };
    int aniTileDelays2[14]; //= { 37, 2, 1, 2, 2, 6, 2, 12, 2, 1, 2, 2, 6, 2 };
    int field_58;
    bool32 flag;
    int petalTimer;
    int aniTilesDelayC;
    int aniTilesFrameC;
    int aniTilesDelayD;
    int aniTilesFrameD;
    int aniTilesDelayE;
    int aniTilesFrameE;
    int aniTilesDelayF;
    int aniTilesFrameF;
    int aniTilesDelayG;
    int aniTilesFrameG;
    int aniTilesDelayH;
    int aniTilesFrameH;
    int aniTilesFrameI;
    int aniTilesFrameB;
    int aniTilesFrameA;
    int field_A0;
    bool32 hasAchievement;
    ushort aniTilesA;
    ushort aniTilesB;
    ushort aniTilesC;
} ObjectPSZ1Setup;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPSZ1Setup;

// Object Struct
extern ObjectPSZ1Setup *PSZ1Setup;

// Standard Entity Events
void PSZ1Setup_Update(void);
void PSZ1Setup_LateUpdate(void);
void PSZ1Setup_StaticUpdate(void);
void PSZ1Setup_Draw(void);
void PSZ1Setup_Create(void* data);
void PSZ1Setup_StageLoad(void);
void PSZ1Setup_EditorDraw(void);
void PSZ1Setup_EditorLoad(void);
void PSZ1Setup_Serialize(void);

// Extra Entity Functions
void PSZ1Setup_BGSwitchCB1(void);
void PSZ1Setup_BGSwitchCB2(void);
void PSZ1Setup_TriggerCB1(void);
void PSZ1Setup_TriggerCB2(void);
void PSZ1Setup_TriggerCB3(void);
void PSZ1Setup_ActTransitionCB(void);
void PSZ1Setup_LevelWrap_Top(void);
void PSZ1Setup_LevelWrap_Bottom(void);

#endif //!OBJ_PSZ1SETUP_H
