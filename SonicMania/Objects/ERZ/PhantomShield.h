#ifndef OBJ_PHANTOMSHIELD_H
#define OBJ_PHANTOMSHIELD_H

#include "Game.h"

// Object Class
struct ObjectPhantomShield {
    RSDK_OBJECT
    Hitbox hitbox;
    uint16 aniFrames;
};

// Entity Class
struct EntityPhantomShield {
    RSDK_ENTITY
    StateMachine(state);
    bool32 shieldActive;
    int32 blendAmount;
    int32 playerTimer[PLAYER_COUNT];
    Entity *parent;
    Animator animator;
};

// Object Struct
extern ObjectPhantomShield *PhantomShield;

// Standard Entity Events
void PhantomShield_Update(void);
void PhantomShield_LateUpdate(void);
void PhantomShield_StaticUpdate(void);
void PhantomShield_Draw(void);
void PhantomShield_Create(void *data);
void PhantomShield_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PhantomShield_EditorDraw(void);
void PhantomShield_EditorLoad(void);
#endif
void PhantomShield_Serialize(void);

// Extra Entity Functions
void PhantomShield_State_Appear(void);
void PhantomShield_State_Active(void);
void PhantomShield_State_Disappear(void);

#endif //! OBJ_PHANTOMSHIELD_H
