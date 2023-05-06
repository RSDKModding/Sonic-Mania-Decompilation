#ifndef OBJ_UFO_CAMERA_H
#define OBJ_UFO_CAMERA_H

#include "Game.h"

// Object Class
struct ObjectUFO_Camera {
    RSDK_OBJECT
    Matrix matWorld;
    Matrix matView;
    Matrix matTemp;
    bool32 isSS7;
};

// Entity Class
struct EntityUFO_Camera {
    RSDK_ENTITY
    StateMachine(state);
    int32 unused1;
    int32 height;
    int32 radius;
    int32 angleX;
    int32 prevAngle;
    int32 clipY;
    Entity *target;
    Matrix matWorld;
    Matrix matView;
};

// Object Struct
extern ObjectUFO_Camera *UFO_Camera;

// Standard Entity Events
void UFO_Camera_Update(void);
void UFO_Camera_LateUpdate(void);
void UFO_Camera_StaticUpdate(void);
void UFO_Camera_Draw(void);
void UFO_Camera_Create(void *data);
void UFO_Camera_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UFO_Camera_EditorDraw(void);
void UFO_Camera_EditorLoad(void);
#endif
void UFO_Camera_Serialize(void);

// Extra Entity Functions
void UFO_Camera_HandleCamPos(void);
void UFO_Camera_State_Normal(void);
void UFO_Camera_State_CourseOut(void);
void UFO_Camera_State_UFOCaught(void);

#endif //! OBJ_UFO_CAMERA_H
