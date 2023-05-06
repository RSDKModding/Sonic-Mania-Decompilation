#ifndef OBJ_FIREWORK_H
#define OBJ_FIREWORK_H

#include "Game.h"

// Object Class
struct ObjectFirework {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxRocketJet;
    uint16 sfxFlame;
    uint16 sfxExplosion2;
    uint16 sfxExplosion;
    bool32 playingFlameSfx;
    Hitbox hitboxFireworkGrab;
    Hitbox hitboxTileCheck;
};

// Entity Class
struct EntityFirework {
    RSDK_ENTITY
    int32 distance;
    int32 innerRadius;
    int32 outerRadius;
    StateMachine(state);
    uint8 activePlayers;
    int32 playerTimers[PLAYER_COUNT];
    int32 sparkID;
    bool32 activated;
    int32 timer;
    int32 moveVelocity;
    int32 ridePos;
    Vector2 startPos;
    Vector2 lastPosition;
    bool32 rideActive;
    Animator mainAnimator;
    Animator sparkAnimator;
};

// Object Struct
extern ObjectFirework *Firework;

// Standard Entity Events
void Firework_Update(void);
void Firework_LateUpdate(void);
void Firework_StaticUpdate(void);
void Firework_Draw(void);
void Firework_Create(void *data);
void Firework_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Firework_EditorDraw(void);
void Firework_EditorLoad(void);
#endif
void Firework_Serialize(void);

// Extra Entity Functions
void Firework_CheckPlayerCollisions(void);
void Firework_HandlePlayerTimers(void);
void Firework_RemovePlayers(bool32 crashed);
void Firework_HandlePlayerRemoval(EntityPlayer *player, bool32 crashed);
void Firework_HandlePlayerJump(void);
void Firework_HandlePlayerControl(void);
void Firework_HandleMoveDir(void);
void Firework_HandleDebrisSpawn(void);
void Firework_HandleTileCollisions(void);
void Firework_HandleRideEnd(bool32 crashed);
void Firework_CheckOffScreen(void);
void Firework_HandleSparkAnimations(int32 sparkID, int16 angle, Animator *fireworkAnimator, Animator *sparkAnimator, uint8 *direction);

void Firework_State_Init(void);
void Firework_State_AwaitPlayerRide(void);
void Firework_State_PlayerGrabDelay(void);
void Firework_State_Riding(void);
void Firework_State_Explode(void);
void Firework_State_ResetOffScreen(void);

void Firework_State_SparkSetup(void);
void Firework_State_Spark(void);

#endif //! OBJ_FIREWORK_H
