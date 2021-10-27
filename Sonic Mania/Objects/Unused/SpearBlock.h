#ifndef OBJ_SPEARBLOCK_H
#define OBJ_SPEARBLOCK_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox blockHitbox;
    Hitbox spearHitboxes[4];
    uint16 spriteIndex;
    Animator animator;
} ObjectSpearBlock;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    uint8 spearDir;
    Vector2 spearPos;
    int32 timer;
    Animator animator;
} EntitySpearBlock;

// Object Entity
extern ObjectSpearBlock *SpearBlock;

// Standard Entity Events
void SpearBlock_Update(void);
void SpearBlock_LateUpdate(void);
void SpearBlock_StaticUpdate(void);
void SpearBlock_Draw(void);
void SpearBlock_Create(void* data);
void SpearBlock_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void SpearBlock_EditorDraw(void);
void SpearBlock_EditorLoad(void);
#endif
void SpearBlock_Serialize(void);

// Extra Entity Functions
void SpearBlock_DebugDraw(void);
void SpearBlock_DebugSpawn(void);

void SpearBlock_CheckPlayerCollisions(void);

void SpearBlock_State_SetupSpears(void);
void SpearBlock_State_CheckSpearExtend(void);
void SpearBlock_State_ExtendSpears(void);
void SpearBlock_State_CheckSpearRetract(void);
void SpearBlock_State_RetractSpears(void);

#endif //!OBJ_SPEARBLOCK_H
