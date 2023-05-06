#ifndef OBJ_MSBOMB_H
#define OBJ_MSBOMB_H

#include "Game.h"

#if MANIA_USE_PLUS
// Object Class
struct ObjectMSBomb {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxExplosion;
};

// Entity Class
struct EntityMSBomb {
    RSDK_ENTITY
    StateMachine(state);
    int32 unused;
    int32 timer;
    Hitbox hitbox;
    Animator animator;
};

// Object Struct
extern ObjectMSBomb *MSBomb;

// Standard Entity Events
void MSBomb_Update(void);
void MSBomb_LateUpdate(void);
void MSBomb_StaticUpdate(void);
void MSBomb_Draw(void);
void MSBomb_Create(void *data);
void MSBomb_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void MSBomb_EditorDraw(void);
void MSBomb_EditorLoad(void);
#endif
void MSBomb_Serialize(void);

// Extra Entity Functions
void MSBomb_State_EnterBomb(void);
void MSBomb_State_SilverSonicExplode(void);
void MSBomb_State_Bouncing(void);
void MSBomb_State_Projectile(void);

#endif

#endif //! OBJ_MSBOMB_H
