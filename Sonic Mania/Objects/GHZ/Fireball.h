#ifndef OBJ_FIREBALL_H
#define OBJ_FIREBALL_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitbox;
    uint16 sfxFireball;
} ObjectFireball;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    Animator animator;
    uint8 type;
    uint8 interval;
    uint8 intervalOffset;
    int field_7C;
    int field_80;
} EntityFireball;

// Object Entity
extern ObjectFireball *Fireball;

// Standard Entity Events
void Fireball_Update(void);
void Fireball_LateUpdate(void);
void Fireball_StaticUpdate(void);
void Fireball_Draw(void);
void Fireball_Create(void *data);
void Fireball_StageLoad(void);
void Fireball_EditorDraw(void);
void Fireball_EditorLoad(void);
void Fireball_Serialize(void);

// Extra Entity Functions
void Fireball_HandlePlayerInteractions(void);

void Fireball_State_Unknown1(void);
void Fireball_State_Unknown2(void);
void Fireball_State_Unknown3(void);
void Fireball_State_Unknown4(void);
void Fireball_State_Unknown5(void);
void Fireball_State_Unknown6(void);
void Fireball_State_Unknown7(void);

void Fireball_StateDraw_Normal(void);

#endif //! OBJ_FIREBALL_H
