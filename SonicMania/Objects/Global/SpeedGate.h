#ifndef OBJ_SPEEDGATE_H
#define OBJ_SPEEDGATE_H

#include "Game.h"

// Object Class
struct ObjectSpeedGate {
    RSDK_OBJECT
    Hitbox hitboxGate;
    int32 unused;
    uint16 aniFrames;
    uint16 sfxStarPost;
};

// Entity Class
struct EntitySpeedGate {
    RSDK_ENTITY
    StateMachine(state);
    int32 id;
    int32 timer;
    int32 prize;
    int32 baseRotation;
    int32 timerRotation;
    int32 angleVel;
    int32 activeTimer;
    Vector2 timerPos;
    Vector2 playerDistance;
    EntityPlayer *triggerPlayer;
    Animator baseAnimator;
    Animator topAnimator;
    Animator timerAnimator;
    Animator finsAnimator;
    Animator bubbleAnimator;
    int32 interactedPlayers;
};

// Object Struct
extern ObjectSpeedGate *SpeedGate;

// Standard Entity Events
void SpeedGate_Update(void);
void SpeedGate_LateUpdate(void);
void SpeedGate_StaticUpdate(void);
void SpeedGate_Draw(void);
void SpeedGate_Create(void *data);
void SpeedGate_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void SpeedGate_EditorDraw(void);
void SpeedGate_EditorLoad(void);
#endif
void SpeedGate_Serialize(void);

// Extra Entity Functions
void SpeedGate_State_WaitForStart(void);
void SpeedGate_State_ProcessGate(void);
void SpeedGate_State_HandleFinished(void);

#endif //! OBJ_SPEEDGATE_H
