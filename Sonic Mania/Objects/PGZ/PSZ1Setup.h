#ifndef OBJ_PSZ1SETUP_H
#define OBJ_PSZ1SETUP_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(int32 aniTileDelays1[7], { 64, 2, 1, 2, 2, 6, 2 });
    TABLE(int32 aniTileDelays2[14], { 37, 2, 1, 2, 2, 6, 2, 12, 2, 1, 2, 2, 6, 2 });
    int32 field_58;
    bool32 flag;
    int32 petalTimer;
    int32 aniTilesDelayC;
    int32 aniTilesFrameC;
    int32 aniTilesDelayD;
    int32 aniTilesFrameD;
    int32 aniTilesDelayE;
    int32 aniTilesFrameE;
    int32 aniTilesDelayF;
    int32 aniTilesFrameF;
    int32 aniTilesDelayG;
    int32 aniTilesFrameG;
    int32 aniTilesDelayH;
    int32 aniTilesFrameH;
    int32 aniTilesFrameI;
    int32 aniTilesFrameB;
    int32 aniTilesFrameA;
    int32 field_A0;
    bool32 hasAchievement;
    uint16 aniTilesA;
    uint16 aniTilesB;
    uint16 aniTilesC;
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
