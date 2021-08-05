#ifndef OBJ_PBL_CRANE_H
#define OBJ_PBL_CRANE_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    int prizeID;
    ushort aniFrames;
    ushort sfxBackGlass;
    ushort sfxCraneMove;
    ushort sfxCraneDrop;
    ushort sfxCraneGrab;
    ushort sfxCraneRise;
    ushort sfxPrizeGood;
    ushort sfxPrizeBad;
} ObjectPBL_Crane;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int type;
    int field_64;
    int timer;
    int field_6C;
    int field_70;
    Entity *parent;
    Entity *field_78;
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
void PBL_Crane_EditorDraw(void);
void PBL_Crane_EditorLoad(void);
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
