#ifndef OBJ_FIREWORM_H
#define OBJ_FIREWORM_H

#include "Game.h"

#define FIREWORM_SEGMENT_COUNT (1 + 4) // Head + 4 Segments

// Object Class
struct ObjectFireworm {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitboxRange;
    Animator holeAnimator;
    uint16 aniFrames;
};

// Entity Class
struct EntityFireworm {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 startPos;
    uint8 startDir;
    Vector2 bodyPositions[FIREWORM_SEGMENT_COUNT];
    Vector2 bodyVelocities[FIREWORM_SEGMENT_COUNT];
    uint8 bodyDirections[FIREWORM_SEGMENT_COUNT];
    uint16 bodyAngles[FIREWORM_SEGMENT_COUNT];
    uint8 bodyTimers[FIREWORM_SEGMENT_COUNT];
    uint8 flameExhaustDelays[FIREWORM_SEGMENT_COUNT];
    int32 bodyOriginY[FIREWORM_SEGMENT_COUNT];
    int32 timer;
    int32 boundsL;
    int32 boundsR;
    Animator bodyAnimators[FIREWORM_SEGMENT_COUNT];
    Animator flameAnimators[FIREWORM_SEGMENT_COUNT];
};

// Object Struct
extern ObjectFireworm *Fireworm;

// Standard Entity Events
void Fireworm_Update(void);
void Fireworm_LateUpdate(void);
void Fireworm_StaticUpdate(void);
void Fireworm_Draw(void);
void Fireworm_Create(void *data);
void Fireworm_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Fireworm_EditorDraw(void);
void Fireworm_EditorLoad(void);
#endif
void Fireworm_Serialize(void);

// Extra Entity Functions
void Fireworm_DebugSpawn(void);
void Fireworm_DebugDraw(void);

void Fireworm_CheckPlayerCollisions(void);
void Fireworm_CheckOffScreen(void);

void Fireworm_State_Init(void);
void Fireworm_State_AwaitPlayer(void);
void Fireworm_State_HeadAppear(void);

void Fireworm_State_BodyAppear(void);
void Fireworm_State_FlyAround(void);

#endif //! OBJ_FIREWORM_H
