#ifndef OBJ_FLOWERPOD_H
#define OBJ_FLOWERPOD_H

#include "Game.h"

// Object Class
struct ObjectFlowerPod {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitboxPod;
    int32 unused;
    uint16 sfxExplosion;
    uint16 sfxTwinkle;
};

// Entity Class
struct EntityFlowerPod {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    Vector2 startPos;
    Animator stemAnimator;
    Animator podAnimator;
};

// Object Struct
extern ObjectFlowerPod *FlowerPod;

// Standard Entity Events
void FlowerPod_Update(void);
void FlowerPod_LateUpdate(void);
void FlowerPod_StaticUpdate(void);
void FlowerPod_Draw(void);
void FlowerPod_Create(void *data);
void FlowerPod_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void FlowerPod_EditorDraw(void);
void FlowerPod_EditorLoad(void);
#endif
void FlowerPod_Serialize(void);

// Extra Entity Functions
void FlowerPod_SpawnSeeds(void);

void FlowerPod_State_Pod(void);
void FlowerPod_State_Exploding(void);
void FlowerPod_State_Destroyed(void);
void FlowerPod_State_SeedFall(void);
void FlowerPod_State_SpawnBeanstalk(void);
void FlowerPod_State_FlowerGrow(void);
void FlowerPod_State_Flower(void);

#endif //! OBJ_FLOWERPOD_H
