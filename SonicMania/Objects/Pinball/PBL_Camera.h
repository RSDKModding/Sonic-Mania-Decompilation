#ifndef OBJ_PBL_CAMERA_H
#define OBJ_PBL_CAMERA_H

#include "Game.h"

#if MANIA_USE_PLUS
// Object Class
struct ObjectPBL_Camera {
    RSDK_OBJECT
    Matrix matWorld;
    Matrix matNormalItem;
    Matrix matNormal;
    bool32 useAltMatNormal;
};

// Entity Class
struct EntityPBL_Camera {
    RSDK_ENTITY
    StateMachine(state);
    int32 unused1;
    int32 worldY;
    int32 rotationY;
    int32 cameraStartOffset; // I don't think this is ever actually set
    int32 curCamBoundaryT;
    int32 curCamBoundaryB;
    int32 newCamBoundaryT;
    int32 newCamBoundaryB;
    int32 prevAngle;
    int32 centerY;
    Entity *target;
    Matrix matTransform;
    Matrix unusedMatrix1;
    Vector2 targetPos;
};

// Object Struct
extern ObjectPBL_Camera *PBL_Camera;

// Standard Entity Events
void PBL_Camera_Update(void);
void PBL_Camera_LateUpdate(void);
void PBL_Camera_StaticUpdate(void);
void PBL_Camera_Draw(void);
void PBL_Camera_Create(void *data);
void PBL_Camera_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PBL_Camera_EditorDraw(void);
void PBL_Camera_EditorLoad(void);
#endif
void PBL_Camera_Serialize(void);

// Extra Entity Functions
void PBL_Camera_HandleScreenPos(void);
void PBL_Camera_State_Normal(void);

#endif

#endif //! OBJ_PBL_CAMERA_H
