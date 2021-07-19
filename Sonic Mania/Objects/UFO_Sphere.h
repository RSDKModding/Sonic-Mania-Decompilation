#ifndef OBJ_UFO_SPHERE_H
#define OBJ_UFO_SPHERE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int field_4;
    int defaultHeights[4];
    ushort spriteIndex;
    ushort sfx_MachSpeed;
    ushort sfx_LedgeBreak;
} ObjectUFO_Sphere;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int type;
    int behavior;
    int height;
    int speed;
    Vector2 matAngle;
    Vector2 amplitude;
    int timer;
    int dword80;
    int dword84;
    int dword88;
    Vector2 worldPos;
    Vector2 drawPos;
    int dword9C;
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
