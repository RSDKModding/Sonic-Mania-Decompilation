#ifndef OBJ_FIREBALL_H
#define OBJ_FIREBALL_H

#include "SonicMania.h"

typedef enum {
    FIREBALL_UPDOWN,
    FIREBALL_MOVE,
    FIREBALL_MOVEGRAVITY,
}FireballTypes;

// Object Class
struct ObjectFireball {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitbox;
    uint16 sfxFireball;
};

// Entity Class
struct EntityFireball {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    Animator animator;
    uint8 type;
    uint8 interval;
    uint8 intervalOffset;
    int32 unused1;
    int32 unused2;
};

// Object Entity
extern ObjectFireball *Fireball;

// Standard Entity Events
void Fireball_Update(void);
void Fireball_LateUpdate(void);
void Fireball_StaticUpdate(void);
void Fireball_Draw(void);
void Fireball_Create(void *data);
void Fireball_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Fireball_EditorDraw(void);
void Fireball_EditorLoad(void);
#endif
void Fireball_Serialize(void);

// Extra Entity Functions
void Fireball_HandlePlayerInteractions(void);

void Fireball_State_UpDown_Spawner(void);
void Fireball_State_Move_Spawner(void);
void Fireball_State_MoveGravity_Spawner(void);
void Fireball_State_UpDown(void);
void Fireball_State_Move(void);
void Fireball_State_MoveGravity(void);
void Fireball_State_Dissipate(void);

void Fireball_StateDraw_Normal(void);

#endif //! OBJ_FIREBALL_H
