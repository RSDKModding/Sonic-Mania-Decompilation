#ifndef OBJ_DUST_H
#define OBJ_DUST_H

#include "SonicMania.h"

// Object Class
struct ObjectDust {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityDust {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    Entity *parent;
    Animator animator;
};

// Object Struct
extern ObjectDust *Dust;

// Standard Entity Events
void Dust_Update(void);
void Dust_LateUpdate(void);
void Dust_StaticUpdate(void);
void Dust_Draw(void);
void Dust_Create(void *data);
void Dust_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Dust_EditorDraw(void);
void Dust_EditorLoad(void);
#endif
void Dust_Serialize(void);

// Extra Entity Functions
void Dust_State_Spindash(void);
void Dust_State_Skid(void);
void Dust_State_GlideSlide(void);
void Dust_State_Move(void);
void Dust_State_MoveCollide(void);
void Dust_State_MoveFriction(void);

#endif //! OBJ_DUST_H
