#ifndef OBJ_UFO_CAMERA_H
#define OBJ_UFO_CAMERA_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Matrix matWorld;
    Matrix matView;
    Matrix matTemp;
    bool32 isSS7;
} ObjectUFO_Camera;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 field_5C;
    int32 height;
    int32 field_64;
    int32 angleX;
    int32 dword6C;
    int32 clipY;
    Entity *target;
    Matrix matWorld;
    Matrix matView;
} EntityUFO_Camera;

// Object Struct
extern ObjectUFO_Camera *UFO_Camera;

// Standard Entity Events
void UFO_Camera_Update(void);
void UFO_Camera_LateUpdate(void);
void UFO_Camera_StaticUpdate(void);
void UFO_Camera_Draw(void);
void UFO_Camera_Create(void* data);
void UFO_Camera_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void UFO_Camera_EditorDraw(void);
void UFO_Camera_EditorLoad(void);
#endif
void UFO_Camera_Serialize(void);

// Extra Entity Functions
void UFO_Camera_Unknown1(void);
void UFO_Camera_Unknown2(void);
void UFO_Camera_Unknown3(void);
void UFO_Camera_Unknown4(void);

#endif //!OBJ_UFO_CAMERA_H
