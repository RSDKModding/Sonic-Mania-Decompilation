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
    void (*state)();
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
    AnimationData data;
} EntityUFO_Sphere;

// Object Struct
extern ObjectUFO_Sphere *UFO_Sphere;

// Standard Entity Events
void UFO_Sphere_Update();
void UFO_Sphere_LateUpdate();
void UFO_Sphere_StaticUpdate();
void UFO_Sphere_Draw();
void UFO_Sphere_Create(void* data);
void UFO_Sphere_StageLoad();
void UFO_Sphere_EditorDraw();
void UFO_Sphere_EditorLoad();
void UFO_Sphere_Serialize();

// Extra Entity Functions
void UFO_Sphere_CheckPlayerCollision();
void UFO_Sphere_State_Bouncing();
void UFO_Sphere_State_Moving();
void UFO_Sphere_Unknown3();
void UFO_Sphere_Unknown4();
void UFO_Sphere_Unknown5();

#endif //!OBJ_UFO_SPHERE_H
