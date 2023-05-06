#ifndef OBJ_TIMEPOST_H
#define OBJ_TIMEPOST_H

#include "Game.h"

// Object Class
struct ObjectTimePost {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sparkleFrames;
    Hitbox hitbox;
    Hitbox hitboxItemBox;
    uint16 sfxFuture;
};

// Entity Class
struct EntityTimePost {
    RSDK_ENTITY
    StateMachine(state);
    uint8 type;
    int32 unused1;
    int32 gravityStrength;
    int32 spinCount;
    int32 spinSpeed;
    int32 maxAngle;
    int32 unused2;
    int32 activePlayers;
    Animator faceplateAnimator;
    Animator sideAnimator;
    Animator standAnimator;
};

// Object Struct
extern ObjectTimePost *TimePost;

// Standard Entity Events
void TimePost_Update(void);
void TimePost_LateUpdate(void);
void TimePost_StaticUpdate(void);
void TimePost_Draw(void);
void TimePost_Create(void *data);
void TimePost_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void TimePost_EditorDraw(void);
void TimePost_EditorLoad(void);
#endif
void TimePost_Serialize(void);

// Extra Entity Functions
void TimePost_Spin(void);
void TimePost_CheckPlayerCollisions(void);
void TimePost_Particle_TimeSparkle(EntityDebris *debris);
void TimePost_HandleTimeSparkles(void);

void TimePost_State_Init(void);
void TimePost_State_CheckPlayerCollisions(void);
void TimePost_State_Spinning(void);
void TimePost_State_FinishedSpin(void);

#endif //! OBJ_TIMEPOST_H
