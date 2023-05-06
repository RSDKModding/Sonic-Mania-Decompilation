#ifndef OBJ_UFO_SPHERE_H
#define OBJ_UFO_SPHERE_H

#include "Game.h"

typedef enum {
    UFO_SPHERE_BLUE,
    UFO_SPHERE_RED,
    UFO_SPHERE_MINE,
    UFO_SPHERE_SPIKES,
} UFO_SphereTypes;

typedef enum {
    UFO_SPHERE_B_NONE,
    UFO_SPHERE_B_BOUNCE,
    UFO_SPHERE_B_MOVE,
} UFO_SphereBehaviours;

// Object Class
struct ObjectUFO_Sphere {
    RSDK_OBJECT
    int32 unused;
    TABLE(int32 defaultHeights[4], { 12, 12, 16, 16 });
    uint16 aniFrames;
    uint16 sfxMachSpeed;
    uint16 sfxLedgeBreak;
};

// Entity Class
struct EntityUFO_Sphere {
    RSDK_ENTITY
    StateMachine(state);
    int32 type;
    int32 behavior;
    int32 height;
    int32 speed;
    Vector2 matAngle;
    Vector2 amplitude;
    int32 timer;
    int32 startHeight;
    Vector2 startPos;
    Vector2 worldPos;
    Vector2 drawPos;
    int32 scaleFactor;
    Matrix matrix;
    Matrix matrix2;
    Animator animator;
};

// Object Struct
extern ObjectUFO_Sphere *UFO_Sphere;

// Standard Entity Events
void UFO_Sphere_Update(void);
void UFO_Sphere_LateUpdate(void);
void UFO_Sphere_StaticUpdate(void);
void UFO_Sphere_Draw(void);
void UFO_Sphere_Create(void *data);
void UFO_Sphere_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UFO_Sphere_EditorDraw(void);
void UFO_Sphere_EditorLoad(void);
#endif
void UFO_Sphere_Serialize(void);

// Extra Entity Functions
void UFO_Sphere_State_Fixed(void);
void UFO_Sphere_State_Bouncing(void);
void UFO_Sphere_State_Moving(void);
void UFO_Sphere_State_Collected(void);
void UFO_Sphere_State_HandleRespawn(void);
void UFO_Sphere_State_AnimateAndDestroy(void);

#endif //! OBJ_UFO_SPHERE_H
