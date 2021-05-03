#ifndef OBJ_PBL_CAMERA_H
#define OBJ_PBL_CAMERA_H

#include "../SonicMania.h"

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
    int dword5C;
    int worldY;
    int rotationY;
    int dword68;
    int dword6C;
    int dword70;
    int dword74;
    int dword78;
    int field_7C;
    int field_80;
    Entity *targetPtr;
    Matrix matrix;
    int field_C8;
    int field_CC;
    int field_D0;
    int field_D4;
    int field_D8;
    int field_DC;
    int field_E0;
    int field_E4;
    int field_E8;
    int field_EC;
    int field_F0;
    int field_F4;
    int field_F8;
    int field_FC;
    int field_100;
    int field_104;
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
