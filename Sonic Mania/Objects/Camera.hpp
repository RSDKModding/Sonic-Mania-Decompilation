#ifndef OBJ_CAMERA_H
#define OBJ_CAMERA_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectCamera : Object{
    Vector2 centerBounds;
};

// Entity Class
struct EntityCamera : Entity {
    void (*state)(void);
    Vector2 *targetPosPtr;
    int screenID;
    Vector2 center;
    Vector2 field_6C;
    Vector2 lastPos;
    Vector2 shakePos;
    Vector2 lookPos;
    int field_8C;
    int field_90;
    int field_94;
    int field_98;
    int adjustY;
    int field_A0;
    int field_A4;
    int field_A8;
    Vector2 field_AC;
    int field_B4;
    int field_B8;
    Vector2 boundsOffset;
    int boundsL;
    int boundsR;
    int boundsT;
    int boundsB;
};

// Object Struct
extern ObjectCamera *Camera;

// Standard Entity Events
void Camera_Update();
void Camera_LateUpdate();
void Camera_StaticUpdate();
void Camera_Draw();
void Camera_Create(void* data);
void Camera_StageLoad();
void Camera_EditorDraw();
void Camera_EditorLoad();
void Camera_Serialize();

// Extra Entity Functions
void Camera_SetCameraBounds(EntityCamera *entity);
EntityCamera *Camera_SetTargetEntity(int screen, Entity *target);
void Camera_ShakeScreen(int shakeX, int screen, int shakeY);
void Camera_HandleHBounds();
void Camera_HandleVBounds();

//States
void Camera_State_Roam();
void Camera_State_Follow();
void Camera_State_HLock();
void Camera_State_VLock();

#endif //!OBJ_CAMERA_H
