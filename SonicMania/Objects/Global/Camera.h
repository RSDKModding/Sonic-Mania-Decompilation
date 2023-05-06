#ifndef OBJ_CAMERA_H
#define OBJ_CAMERA_H

#include "Game.h"

typedef enum {
    CAMERA_LERP_NORMAL,
    CAMERA_LERP_SIN1024,
    CAMERA_LERP_SIN1024_2,
    CAMERA_LERP_SIN512,
} CameraLerpType;

// Object Class
struct ObjectCamera {
    RSDK_OBJECT
    Vector2 centerBounds;
};

// Entity Class
struct EntityCamera {
    RSDK_ENTITY
    StateMachine(state);
    Entity *target;
    int32 screenID;
    Vector2 center;
    Vector2 targetMoveVel;
    Vector2 lastPos;
    Vector2 shakePos;
    Vector2 lookPos;
    Vector2 offset;
    bool32 disableYOffset;
    int32 centerY;
    int32 adjustY;
    int32 lerpPercent;
    int32 lerpSpeed;
    int32 lerpType;
    Vector2 endLerpPos;
    Vector2 startLerpPos;
    Vector2 boundsOffset;
    int32 boundsL;
    int32 boundsR;
    int32 boundsT;
    int32 boundsB;
};

// Object Struct
extern ObjectCamera *Camera;

// Standard Entity Events
void Camera_Update(void);
void Camera_LateUpdate(void);
void Camera_StaticUpdate(void);
void Camera_Draw(void);
void Camera_Create(void *data);
void Camera_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Camera_EditorDraw(void);
void Camera_EditorLoad(void);
#endif
void Camera_Serialize(void);

// Extra Entity Functions
void Camera_SetCameraBounds(EntityCamera *entity);
EntityCamera *Camera_SetTargetEntity(int32 screen, void *t);
void Camera_ShakeScreen(int32 screen, int32 shakeX, int32 shakeY);
void Camera_HandleHBounds(void);
void Camera_HandleVBounds(void);
void Camera_SetupLerp(int32 type, int32 screen, int32 x, int32 y, int32 speed);

// States
void Camera_State_MapView(void);
void Camera_State_FollowXY(void);
void Camera_State_FollowX(void);
void Camera_State_FollowY(void);
void Camera_State_HandleLerp(void);

#endif //! OBJ_CAMERA_H
