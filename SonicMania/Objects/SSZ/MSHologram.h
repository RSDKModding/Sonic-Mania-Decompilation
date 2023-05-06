#ifndef OBJ_MSHOLOGRAM_H
#define OBJ_MSHOLOGRAM_H

#include "Game.h"

// Object Class
struct ObjectMSHologram {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitbox;
    uint16 sfxExplosion;
};

// Entity Class
struct EntityMSHologram {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    bool32 destroyed;
    Animator projectorAnimator;
    Animator lightsAnimator;
    Animator metalSonicAnimator;
    Animator rabbitAnimator;
    Animator canaryAnimator;
};

// Object Struct
extern ObjectMSHologram *MSHologram;

// Standard Entity Events
void MSHologram_Update(void);
void MSHologram_LateUpdate(void);
void MSHologram_StaticUpdate(void);
void MSHologram_Draw(void);
void MSHologram_Create(void *data);
void MSHologram_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void MSHologram_EditorDraw(void);
void MSHologram_EditorLoad(void);
#endif
void MSHologram_Serialize(void);

// Extra Entity Functions
void MSHologram_State_CheckPlayerCollisions(void);
void MSHologram_State_Explode(void);
void MSHologram_State_Destroyed(void);

#endif //! OBJ_MSHOLOGRAM_H
