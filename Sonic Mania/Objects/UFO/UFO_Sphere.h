#ifndef OBJ_UFO_SPHERE_H
#define OBJ_UFO_SPHERE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 field_4;
    TABLE(int32 defaultHeights[4], { 12, 12, 16, 16 });
    uint16 spriteIndex;
    uint16 sfx_MachSpeed;
    uint16 sfx_LedgeBreak;
} ObjectUFO_Sphere;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 type;
    int32 behavior;
    int32 height;
    int32 speed;
    Vector2 matAngle;
    Vector2 amplitude;
    int32 timer;
    int32 dword80;
    int32 dword84;
    int32 dword88;
    Vector2 worldPos;
    Vector2 drawPos;
    int32 dword9C;
    Matrix matrix;
    Matrix matrix2;
    Animator animator;
} EntityUFO_Sphere;

// Object Struct
extern ObjectUFO_Sphere *UFO_Sphere;

// Standard Entity Events
void UFO_Sphere_Update(void);
void UFO_Sphere_LateUpdate(void);
void UFO_Sphere_StaticUpdate(void);
void UFO_Sphere_Draw(void);
void UFO_Sphere_Create(void* data);
void UFO_Sphere_StageLoad(void);
void UFO_Sphere_EditorDraw(void);
void UFO_Sphere_EditorLoad(void);
void UFO_Sphere_Serialize(void);

// Extra Entity Functions
void UFO_Sphere_CheckPlayerCollision(void);
void UFO_Sphere_State_Bouncing(void);
void UFO_Sphere_State_Moving(void);
void UFO_Sphere_Unknown3(void);
void UFO_Sphere_Unknown4(void);
void UFO_Sphere_Unknown5(void);

#endif //!OBJ_UFO_SPHERE_H
