#ifndef OBJ_CACTULA_H
#define OBJ_CACTULA_H

#include "Game.h"

// Object Class
struct ObjectCactula {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    uint16 aniFrames;
    uint16 sfxCactDrop;
};

// Entity Class
struct EntityCactula {
    RSDK_ENTITY
    StateMachine(state);
    int32 unused;
    int32 offsetY;
    bool32 droppedBomb;
    Animator bodyTopAnimator;
    Animator bodyBottomAnimator;
    Animator propellerAnimator;
};

// Object Struct
extern ObjectCactula *Cactula;

// Standard Entity Events
void Cactula_Update(void);
void Cactula_LateUpdate(void);
void Cactula_StaticUpdate(void);
void Cactula_Draw(void);
void Cactula_Create(void *data);
void Cactula_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Cactula_EditorDraw(void);
void Cactula_EditorLoad(void);
#endif
void Cactula_Serialize(void);

// Extra Entity Functions
void Cactula_DebugSpawn(void);
void Cactula_DebugDraw(void);

void Cactula_CheckPlayerCollisions(void);
bool32 Cactula_SfxChecK_CactChopper(void);

void Cactula_State_CheckPlayerInRange(void);
void Cactula_State_Rising(void);
void Cactula_State_DropBomb(void);

#endif //! OBJ_CACTULA_H
