#ifndef OBJ_TIMEPOST_H
#define OBJ_TIMEPOST_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sparkleFrames;
    Hitbox hitbox1;
    Hitbox hitbox2;
    uint16 sfxFuture;
} ObjectTimePost;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    uint8 type;
    int field_60;
    int field_64;
    int spinCount;
    int spinSpeed;
    int maxAngle;
    int field_74;
    int field_78;
    Animator animator1;
    Animator animator2;
    Animator animator3;
} EntityTimePost;

// Object Struct
extern ObjectTimePost *TimePost;

// Standard Entity Events
void TimePost_Update(void);
void TimePost_LateUpdate(void);
void TimePost_StaticUpdate(void);
void TimePost_Draw(void);
void TimePost_Create(void* data);
void TimePost_StageLoad(void);
void TimePost_EditorDraw(void);
void TimePost_EditorLoad(void);
void TimePost_Serialize(void);

// Extra Entity Functions
void TimePost_Spin(void);
void TimePost_CheckPlayerCollisions(void);
void TimePost_DebrisState_TimeSparkles(EntityDebris *entity);
void TimePost_HandleTimeSparkles(void);

void TimePost_State_Setup(void);
void TimePost_State_CheckPlayerCollisions(void);
void TimePost_State_Spin(void);
void TimePost_State_FinishedSpin(void);

#endif //!OBJ_TIMEPOST_H
