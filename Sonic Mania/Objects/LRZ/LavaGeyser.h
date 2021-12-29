#ifndef OBJ_LAVAGEYSER_H
#define OBJ_LAVAGEYSER_H

#include "SonicMania.h"

// Object Class
struct ObjectLavaGeyser {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxHit;
    uint16 sfxExplosion;
    uint16 sfxDrill;
    uint16 sfxLavaGeyser;
};

// Entity Class
struct EntityLavaGeyser {
    RSDK_ENTITY
    StateMachine(state);
    uint8 type;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    int32 field_A8;
    int32 height;
    int32 timer;
    uint16 field_B4;
    uint16 interval;
    uint16 intervalOffset;
    uint16 duration;
    int32 force;
};

// Object Struct
extern ObjectLavaGeyser *LavaGeyser;

// Standard Entity Events
void LavaGeyser_Update(void);
void LavaGeyser_LateUpdate(void);
void LavaGeyser_StaticUpdate(void);
void LavaGeyser_Draw(void);
void LavaGeyser_Create(void* data);
void LavaGeyser_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void LavaGeyser_EditorDraw(void);
void LavaGeyser_EditorLoad(void);
#endif
void LavaGeyser_Serialize(void);

// Extra Entity Functions
void LavaGeyser_CheckPlayerCollisions(void);
void LavaGeyser_HandleSetup(void);
void LavaGeyser_HandleIntervals(void);

void LavaGeyser_State_Unknown1(void);
void LavaGeyser_State_Unknown2(void);
void LavaGeyser_State_Unknown3(void);
void LavaGeyser_State_Unknown4(void);
void LavaGeyser_State_Unknown5(void);
void LavaGeyser_State_Unknown6(void);

#endif //!OBJ_LAVAGEYSER_H
