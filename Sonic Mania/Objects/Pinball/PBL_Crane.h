#ifndef OBJ_PBL_CRANE_H
#define OBJ_PBL_CRANE_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    int32 prizeID;
    uint16 aniFrames;
    uint16 sfxBackGlass;
    uint16 sfxCraneMove;
    uint16 sfxCraneDrop;
    uint16 sfxCraneGrab;
    uint16 sfxCraneRise;
    uint16 sfxPrizeGood;
    uint16 sfxPrizeBad;
} ObjectPBL_Crane;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int32 type;
    int32 field_64;
    int32 timer;
    int32 field_6C;
    int32 field_70;
    Entity *parent;
    Entity *cameraTarget;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    Animator animator5;
} EntityPBL_Crane;

// Object Struct
extern ObjectPBL_Crane *PBL_Crane;

// Standard Entity Events
void PBL_Crane_Update(void);
void PBL_Crane_LateUpdate(void);
void PBL_Crane_StaticUpdate(void);
void PBL_Crane_Draw(void);
void PBL_Crane_Create(void* data);
void PBL_Crane_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void PBL_Crane_EditorDraw(void);
void PBL_Crane_EditorLoad(void);
#endif
void PBL_Crane_Serialize(void);

// Extra Entity Functions
void PBL_Crane_HandlePrizes(void);

void PBL_Crane_StateDraw_Unknown1(void);
void PBL_Crane_StateDraw_Unknown3(void);
void PBL_Crane_StateDraw_Unknown2(void);

void PBL_Crane_State_CreatePrizes(void);
void PBL_Crane_Unknown6(void);
void PBL_Crane_Unknown7(void);

void PBL_Crane_State_Unknown1(void);
void PBL_Crane_State_Unknown2(void);
void PBL_Crane_State_Unknown3(void);
void PBL_Crane_State_Unknown4(void);
void PBL_Crane_State_Unknown5(void);
void PBL_Crane_State_Unknown6(void);

void PBL_Crane_State2_Unknown1(void);
void PBL_Crane_State2_Unknown2(void);
void PBL_Crane_State2_Unknown3(void);

#endif

#endif //!OBJ_PBL_CRANE_H
