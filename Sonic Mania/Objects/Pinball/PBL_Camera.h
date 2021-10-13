#ifndef OBJ_PBL_CAMERA_H
#define OBJ_PBL_CAMERA_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    Matrix matrix1;
    Matrix matrix2;
    Matrix matrix3;
    bool32 flag;
} ObjectPBL_Camera;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 dword5C;
    int32 worldY;
    int32 rotationY;
    int32 dword68;
    int32 dword6C;
    int32 dword70;
    int32 dword74;
    int32 dword78;
    int32 field_7C;
    int32 field_80;
    Entity *targetPtr;
    Matrix matrix;
    int32 field_C8;
    int32 field_CC;
    int32 field_D0;
    int32 field_D4;
    int32 field_D8;
    int32 field_DC;
    int32 field_E0;
    int32 field_E4;
    int32 field_E8;
    int32 field_EC;
    int32 field_F0;
    int32 field_F4;
    int32 field_F8;
    int32 field_FC;
    int32 field_100;
    int32 field_104;
    Vector2 targetPos;
} EntityPBL_Camera;

// Object Struct
extern ObjectPBL_Camera *PBL_Camera;

// Standard Entity Events
void PBL_Camera_Update(void);
void PBL_Camera_LateUpdate(void);
void PBL_Camera_StaticUpdate(void);
void PBL_Camera_Draw(void);
void PBL_Camera_Create(void* data);
void PBL_Camera_StageLoad(void);
void PBL_Camera_EditorDraw(void);
void PBL_Camera_EditorLoad(void);
void PBL_Camera_Serialize(void);

// Extra Entity Functions
void PBL_Camera_Unknown1(void);
void PBL_Camera_Unknown2(void);

#endif

#endif //!OBJ_PBL_CAMERA_H
