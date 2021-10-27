#ifndef OBJ_BUBBLER_H
#define OBJ_BUBBLER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    uint16 aniFrames;
} ObjectBubbler;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    uint16 timer;
    uint16 timer2;
    Vector2 startPos;
    uint8 startDir;
    Animator animator1;
    Animator animator2;
} EntityBubbler;

// Object Entity
extern ObjectBubbler *Bubbler;

// Standard Entity Events
void Bubbler_Update(void);
void Bubbler_LateUpdate(void);
void Bubbler_StaticUpdate(void);
void Bubbler_Draw(void);
void Bubbler_Create(void* data);
void Bubbler_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Bubbler_EditorDraw(void);
void Bubbler_EditorLoad(void);
#endif
void Bubbler_Serialize(void);

// Extra Entity Functions
void Bubbler_DebugSpawn(void);
void Bubbler_DebugDraw(void);

void Bubbler_HandleInteractions(void);
void Bubbler_HandleProjectileInteractions(void);
void Bubbler_CheckOnScreen(void);

void Bubbler_State_Setup(void);
void Bubbler_State_Unknown1(void);
void Bubbler_State_Unknown2(void);
void Bubbler_State_Unknown3(void);

void Bubbler_State_Projectile_Unknown1(void);
void Bubbler_State_Projectile_Unknown2(void);

#endif //!OBJ_BUBBLER_H
