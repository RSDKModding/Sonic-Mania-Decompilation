#ifndef OBJ_LAVAGEYSER_H
#define OBJ_LAVAGEYSER_H

#include "Game.h"

typedef enum {
    LAVAGEYSER_INTERVAL,
    LAVAGEYSER_REPEAT,
} LavaGeyserTypes;

// Object Class
struct ObjectLavaGeyser {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxHit;
    uint16 sfxExplosion;
    uint16 sfxDrill;
    uint16 sfxLavaGeyser;
};

// Entity Class
struct EntityLavaGeyser {
    RSDK_ENTITY
    StateMachine(state);
    uint8 type;
    Animator plumeAnimator;
    Animator flowAnimator;
    Animator plumeLoopAnimator;
    int32 startingHeight;
    int32 height;
    int32 timer;
    uint16 unused;
    uint16 interval;
    uint16 intervalOffset;
    uint16 duration;
    int32 force;
};

// Object Struct
extern ObjectLavaGeyser *LavaGeyser;

// Standard Entity Events
void LavaGeyser_Update(void);
void LavaGeyser_LateUpdate(void);
void LavaGeyser_StaticUpdate(void);
void LavaGeyser_Draw(void);
void LavaGeyser_Create(void *data);
void LavaGeyser_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void LavaGeyser_EditorDraw(void);
void LavaGeyser_EditorLoad(void);
#endif
void LavaGeyser_Serialize(void);

// Extra Entity Functions
void LavaGeyser_CheckPlayerCollisions(void);
void LavaGeyser_HandleSetup(void);

void LavaGeyser_State_Intervals(void);
void LavaGeyser_State_Init(void);
void LavaGeyser_State_ShowPlume(void);
void LavaGeyser_State_Erupting(void);
void LavaGeyser_State_Erupted(void);
void LavaGeyser_State_Recede(void);
void LavaGeyser_State_HandleFinish(void);

#endif //! OBJ_LAVAGEYSER_H
