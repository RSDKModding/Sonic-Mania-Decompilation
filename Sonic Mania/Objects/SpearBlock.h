#ifndef OBJ_SPEARBLOCK_H
#define OBJ_SPEARBLOCK_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox blockHitbox;
    Hitbox spearHitboxes[4];
    ushort spriteIndex;
    AnimationData data;
} ObjectSpearBlock;

// Entity Class
typedef struct {
    RSDK_ENTITY
    void (*state)();
    byte spearDir;
    Vector2 spearPos;
    int timer;
    AnimationData data;
} EntitySpearBlock;

// Object Entity
extern ObjectSpearBlock *SpearBlock;

// Standard Entity Events
void SpearBlock_Update();
void SpearBlock_LateUpdate();
void SpearBlock_StaticUpdate();
void SpearBlock_Draw();
void SpearBlock_Create(void* data);
void SpearBlock_StageLoad();
void SpearBlock_EditorDraw();
void SpearBlock_EditorLoad();
void SpearBlock_Serialize();

// Extra Entity Functions
void SpearBlock_DebugDraw();
void SpearBlock_DebugSpawn();

void SpearBlock_CheckPlayerCollisions();

void SpearBlock_State_SetupSpears();
void SpearBlock_State_CheckSpearExtend();
void SpearBlock_State_ExtendSpears();
void SpearBlock_State_CheckSpearRetract();
void SpearBlock_State_RetractSpears();

#endif //!OBJ_SPEARBLOCK_H
