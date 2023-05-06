#ifndef OBJ_SPEARBLOCK_H
#define OBJ_SPEARBLOCK_H

#include "Game.h"

// Object Class
struct ObjectSpearBlock {
    RSDK_OBJECT
    Hitbox blockHitbox;
    Hitbox spearHitboxes[4];
    uint16 aniFrames;
    Animator animator;
};

// Entity Class
struct EntitySpearBlock {
    RSDK_ENTITY
    StateMachine(state);
    uint8 spearDir;
    Vector2 spearPos;
    int32 timer;
    Animator animator;
};

// Object Entity
extern ObjectSpearBlock *SpearBlock;

// Standard Entity Events
void SpearBlock_Update(void);
void SpearBlock_LateUpdate(void);
void SpearBlock_StaticUpdate(void);
void SpearBlock_Draw(void);
void SpearBlock_Create(void *data);
void SpearBlock_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void SpearBlock_EditorDraw(void);
void SpearBlock_EditorLoad(void);
#endif
void SpearBlock_Serialize(void);

// Extra Entity Functions
void SpearBlock_DebugDraw(void);
void SpearBlock_DebugSpawn(void);

void SpearBlock_CheckPlayerCollisions(void);

void SpearBlock_State_SetupSpears(void);
void SpearBlock_State_SpearRetracted(void);
void SpearBlock_State_ExtendSpears(void);
void SpearBlock_State_SpearExtended(void);
void SpearBlock_State_RetractSpears(void);

#endif //! OBJ_SPEARBLOCK_H
