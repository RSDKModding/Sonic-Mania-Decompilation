#ifndef OBJ_DCEVENT_H
#define OBJ_DCEVENT_H

#include "SonicMania.h"

// Object Class
struct ObjectDCEvent {
    RSDK_OBJECT
    Hitbox hitbox1;
    Hitbox hitbox2;
    bool32 finished;
    uint16 aniFrames;
    uint16 eggmanFrames;
    uint16 sfxExplosion;
    uint16 sfxRumble;
    uint16 sfxImpact6;
    uint16 sfxImpact4;
};

// Entity Class
struct EntityDCEvent {
    RSDK_ENTITY
    StateMachine(state);
    uint8 numChildren;
    uint8 type;
    int32 timer2;
    int32 field_64;
    int32 timer;
    Animator animator;
};

// Object Struct
extern ObjectDCEvent *DCEvent;

// Standard Entity Events
void DCEvent_Update(void);
void DCEvent_LateUpdate(void);
void DCEvent_StaticUpdate(void);
void DCEvent_Draw(void);
void DCEvent_Create(void* data);
void DCEvent_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void DCEvent_EditorDraw(void);
void DCEvent_EditorLoad(void);
#endif
void DCEvent_Serialize(void);

// Extra Entity Functions
void DCEvent_State_Collapse(void);
void DCEvent_State_Unknown1(void);

void DCEvent_StateInput_Unknown1(void);
void DCEvent_StateInput_Unknown2(void);

void DCEvent_State_Unknown2(void);
void DCEvent_State_Unknown3(void);
void DCEvent_State_Unknown4(void);
void DCEvent_State_Unknown5(void);

void DCEvent_State1_Unknown1(void);
void DCEvent_State1_Unknown2(void);

void DCEvent_State2_Unknown1(void);
void DCEvent_State2_Unknown2(void);

#endif //!OBJ_DCEVENT_H
