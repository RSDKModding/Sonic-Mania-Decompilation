#ifndef OBJ_STALACTITE_H
#define OBJ_STALACTITE_H

#include "Game.h"

// Object Class
struct ObjectStalactite {
    RSDK_OBJECT
    Hitbox hitboxStalactite;
    Hitbox hitboxRange;
    uint16 aniFrames;
    uint16 sfxShoot;
};

// Entity Class
struct EntityStalactite {
    RSDK_ENTITY
    StateMachine(state);
    Animator animator;
};

// Object Struct
extern ObjectStalactite *Stalactite;

// Standard Entity Events
void Stalactite_Update(void);
void Stalactite_LateUpdate(void);
void Stalactite_StaticUpdate(void);
void Stalactite_Draw(void);
void Stalactite_Create(void *data);
void Stalactite_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Stalactite_EditorDraw(void);
void Stalactite_EditorLoad(void);
#endif
void Stalactite_Serialize(void);

// Extra Entity Functions
void Stalactite_State_AwaitPlayer(void);
void Stalactite_State_Falling(void);
void Stalactite_State_Landed(void);
void Stalactite_State_Falling_Boss(void);
void Stalactite_State_Debris(void);

#endif //! OBJ_STALACTITE_H
