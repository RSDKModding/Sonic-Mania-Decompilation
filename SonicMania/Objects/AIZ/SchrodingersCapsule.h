#ifndef OBJ_SCHRODINGERSCAPSULE_H
#define OBJ_SCHRODINGERSCAPSULE_H

#include "Game.h"

#if MANIA_USE_PLUS
// Object Class
struct ObjectSchrodingersCapsule {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxExplosion2;
    uint16 sfxExplosion3;
};

// Entity Class
struct EntitySchrodingersCapsule {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 buttonPos;
    // both of these 2 below go unused, but based on the similarities to Global/EggPrison, its likely to be these
    int32 notSolid;
    int32 checkTileCollisions;
    Hitbox hitboxSolid;
    Hitbox hitboxButton;
    Hitbox hitboxButtonTrigger;
    Animator mainAnimator;
    Animator glassAnimator;
    Animator buttonAnimator;
    Animator mightyAnimator;
    Animator rayAnimator;
};

// Object Struct
extern ObjectSchrodingersCapsule *SchrodingersCapsule;

// Standard Entity Events
void SchrodingersCapsule_Update(void);
void SchrodingersCapsule_LateUpdate(void);
void SchrodingersCapsule_StaticUpdate(void);
void SchrodingersCapsule_Draw(void);
void SchrodingersCapsule_Create(void *data);
void SchrodingersCapsule_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void SchrodingersCapsule_EditorDraw(void);
void SchrodingersCapsule_EditorLoad(void);
#endif
void SchrodingersCapsule_Serialize(void);

// Extra Entity Functions
void SchrodingersCapsule_State_Init(void);
void SchrodingersCapsule_State_HandleBounds(void);
void SchrodingersCapsule_State_Activated(void);
void SchrodingersCapsule_State_Explode(void);
void SchrodingersCapsule_State_SetupActClear(void);

#endif

#endif //! OBJ_SCHRODINGERSCAPSULE_H
