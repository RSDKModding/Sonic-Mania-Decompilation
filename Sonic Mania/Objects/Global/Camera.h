#ifndef OBJ_CAMERA_H
#define OBJ_CAMERA_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Vector2 centerBounds;
} ObjectCamera;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    Entity *targetPtr;
    int screenID;
    Vector2 center;
    Vector2 field_6C;
    Vector2 lastPos;
    Vector2 shakePos;
    Vector2 lookPos;
    int field_8C;
    int field_90;
    int field_94;
    int centerY;
    int adjustY;
    int field_A0;
    int field_A4;
    int field_A8;
    Vector2 endLerpPos;
    Vector2 startLerpPos;
    Vector2 boundsOffset;
    int boundsL;
    int boundsR;
    int boundsT;
    int boundsB;
} EntityCamera;

// Object Struct
extern ObjectCamera *Camera;

// Standard Entity Events
void Camera_Update(void);
void Camera_LateUpdate(void);
void Camera_StaticUpdate(void);
void Camera_Draw(void);
void Camera_Create(void* data);
void Camera_StageLoad(void);
void Camera_EditorDraw(void);
void Camera_EditorLoad(void);
void Camera_Serialize(void);

// Extra Entity Functions
void Camera_SetCameraBounds(EntityCamera *entity);
EntityCamera *Camera_SetTargetEntity(int screen, void *t);
void Camera_ShakeScreen(int shakeX, int screen, int shakeY);
void Camera_HandleHBounds(void);
void Camera_HandleVBounds(void);
void Camera_Unknown3(int a1, int screen, int x, int y, int a5);

//States
void Camera_State_Roam(void);
void Camera_State_Follow(void);
void Camera_State_HLock(void);
void Camera_State_VLock(void);
void Camera_State_Unknown(void);

#endif //!OBJ_CAMERA_H
