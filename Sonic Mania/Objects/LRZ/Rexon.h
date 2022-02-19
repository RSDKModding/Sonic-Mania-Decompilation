#ifndef OBJ_REXON_H
#define OBJ_REXON_H

#include "SonicMania.h"

#define Rexon_SegmentCount (5)

// Object Class
struct ObjectRexon {
    RSDK_OBJECT
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    Hitbox hitbox4;
    uint16 aniFrames;
    uint16 sfxShot;
    uint16 sfxExplosion;
};

// Entity Class
struct EntityRexon {
    RSDK_ENTITY
    StateMachine(state);
    bool32 noMove;
    int32 type;
    Vector2 startPos;
    uint8 startDir;
    Vector2 positions[Rexon_SegmentCount + 1];
    int32 field_A0[Rexon_SegmentCount];
    int32 field_B4[Rexon_SegmentCount];
    uint8 field_C8[Rexon_SegmentCount];
    uint8 segmentID;
    int32 timer;
    int32 destroyDelay;
    Animator animator1;
    Animator animator2;
    Animator animator3;
};

// Object Struct
extern ObjectRexon *Rexon;

// Standard Entity Events
void Rexon_Update(void);
void Rexon_LateUpdate(void);
void Rexon_StaticUpdate(void);
void Rexon_Draw(void);
void Rexon_Create(void* data);
void Rexon_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Rexon_EditorDraw(void);
void Rexon_EditorLoad(void);
#endif
void Rexon_Serialize(void);

// Extra Entity Functions
void Rexon_DebugSpawn(void);
void Rexon_DebugDraw(void);

void Rexon_CheckOffScreen(void);
void Rexon_CheckPlayerCollisions(void);
void Rexon_Destroy(EntityRexon *rexon, bool32 crushed);

void Rexon_State_Setup(void);
void Rexon_State_Unknown1(void);
void Rexon_State_Unknown2(void);
void Rexon_State_Unknown3(void);
void Rexon_State_Destroyed(void);
void Rexon_State_Explode(void);

void Rexon_State1_Unknown(void);
void Rexon_State2_Unknown(void);

#endif //!OBJ_REXON_H
