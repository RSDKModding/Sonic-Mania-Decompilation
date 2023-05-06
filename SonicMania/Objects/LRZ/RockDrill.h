#ifndef OBJ_ROCKDRILL_H
#define OBJ_ROCKDRILL_H

#include "Game.h"

// Object Class
struct ObjectRockDrill {
    RSDK_OBJECT
    Hitbox hitboxBody;
    Hitbox hitboxPistonL;
    Hitbox hitboxPistonR;
    Hitbox hitboxDrills;
    uint8 drillSfxTimer;
    bool32 playingDrillSfx;
    uint16 aniFrames;
    uint16 sfxHit;
    uint16 sfxExplosion;
    uint16 sfxDrill;
};

// Entity Class
struct EntityRockDrill {
    RSDK_ENTITY
    StateMachine(state);
    Animator animator;
    bool32 lockCamera;
    int32 boundsL;
    int32 boundsR;
    int32 boundsT;
    int32 boundsB;
    int32 timer;
    int32 pistonPos[2];
    int32 drillPos[2];
    int32 pistonDelay[2];
    int32 drillDelay[2];
    int32 pistonMoveDir[2];
    int32 drillMoveDir[2];
    uint8 invincibilityTimer;
};

// Object Struct
extern ObjectRockDrill *RockDrill;

// Standard Entity Events
void RockDrill_Update(void);
void RockDrill_LateUpdate(void);
void RockDrill_StaticUpdate(void);
void RockDrill_Draw(void);
void RockDrill_Create(void *data);
void RockDrill_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void RockDrill_EditorDraw(void);
void RockDrill_EditorLoad(void);
#endif
void RockDrill_Serialize(void);

// Extra Entity Functions
void RockDrill_CheckPlayerCollisions(void);
void RockDrill_Hit(void);
void RockDrill_Explode(void);
void RockDrill_SpawnDebris(int32 offset);

void RockDrill_State_Init(void);
void RockDrill_State_Drilling(void);
void RockDrill_State_Falling(void);
void RockDrill_State_Explode(void);

#endif //! OBJ_ROCKDRILL_H
