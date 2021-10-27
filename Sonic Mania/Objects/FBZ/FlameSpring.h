#ifndef OBJ_FLAMESPRING_H
#define OBJ_FLAMESPRING_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitbox1;
    Hitbox hitbox2;
    uint16 sfxSpring;
    uint16 sfxFlame;
} ObjectFlameSpring;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int32 type;
    int32 force;
    uint8 playerAni;
    uint8 offset;
    Vector2 field_6C;
    Vector2 field_74;
    bool32 flag;
    uint8 timer;
    Animator animator1;
    Animator animator2;
} EntityFlameSpring;

// Object Struct
extern ObjectFlameSpring *FlameSpring;

// Standard Entity Events
void FlameSpring_Update(void);
void FlameSpring_LateUpdate(void);
void FlameSpring_StaticUpdate(void);
void FlameSpring_Draw(void);
void FlameSpring_Create(void* data);
void FlameSpring_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void FlameSpring_EditorDraw(void);
void FlameSpring_EditorLoad(void);
#endif
void FlameSpring_Serialize(void);

// Extra Entity Functions
void FlameSpring_State_Unknown1(void);
void FlameSpring_State_Unknown2(void);

void FlameSpring_StateDraw_Unknown1(void);
void FlameSpring_StateDraw_Unknown2(void);

#endif //!OBJ_FLAMESPRING_H
