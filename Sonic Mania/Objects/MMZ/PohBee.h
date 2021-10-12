#ifndef OBJ_POHBEE_H
#define OBJ_POHBEE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    ushort aniFrames;
} ObjectPohBee;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 startPos;
    byte startDir;
    int field_68;
    int field_6C;
    int field_70;
    int field_74;
    int field_78;
    int field_7C;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    Hitbox hitboxes[2];
    byte spikeCount;
    Vector2 amplitude;
    PlaneFilterTypes planeFilter;
} EntityPohBee;

// Object Struct
extern ObjectPohBee *PohBee;

// Standard Entity Events
void PohBee_Update(void);
void PohBee_LateUpdate(void);
void PohBee_StaticUpdate(void);
void PohBee_Draw(void);
void PohBee_Create(void* data);
void PohBee_StageLoad(void);
void PohBee_EditorDraw(void);
void PohBee_EditorLoad(void);
void PohBee_Serialize(void);

// Extra Entity Functions
void PohBee_DebugSpawn(void);
void PohBee_DebugDraw(void);

void PohBee_CheckOnScreen(void);
void PohBee_CheckPlayerCollisions(void);

void PohBee_DrawSprites(void);

Vector2 PohBee_GetSpikePos(byte spikeID, byte shift);
void PohBee_SetupHitboxes(void);

void PohBee_State_Setup(void);
void PohBee_State_Move(void);

#endif //!OBJ_POHBEE_H
