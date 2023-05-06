#ifndef OBJ_GIGAMETAL_H
#define OBJ_GIGAMETAL_H

#include "Game.h"

#if MANIA_USE_PLUS

typedef enum {
    GIGAMETAL_HEAD,
    GIGAMETAL_SHOULDER,
    GIGAMETAL_BODY,
    GIGAMETAL_COVER,
    GIGAMETAL_EYE,
    GIGAMETAL_ARMFRONT,
    GIGAMETAL_ARMBACK,
    GIGAMETAL_LASERCHARGE,
    GIGAMETAL_LASEREDGE,
    GIGAMETAL_LASERBEAM,
    GIGAMETAL_SHARD,
} GigaMetalAniIDs;

// Object Class
struct ObjectGigaMetal {
    RSDK_OBJECT
    int32 invincibleTimer;
    int32 explodeTimer;
    uint16 aniFrames;
    uint16 sfxRoar;
    uint16 sfxImpact;
    uint16 sfxTarget;
    uint16 sfxPimpom;
    uint16 sfxCannon;
    Hitbox hitboxLaser;
    Hitbox hitboxHand;
    Hitbox hitboxCore;
    Hitbox hitboxHead;
};

// Entity Class
struct EntityGigaMetal {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    EntityGigaMetal *body;
    EntityGigaMetal *head;
    EntityPhantomRuby *ruby;
    EntityGigaMetal *cover;
    EntityGigaMetal *shoulder;
    EntityGigaMetal *frontArm;
    EntityGigaMetal *backArm;
    EntityMetalSonic *metalSonic;
    int32 timer;
    int32 health;
    int32 attackTimer;
    Vector2 componentPos;
    Vector2 targetPos;
    Animator mainAnimator;
    Animator jointAnimator;
    Animator armAnimator;
    Animator handAnimator;
    uint8 aniID;
    uint8 frameID;
    int32 rotationAngles[3];
    int32 laserSize;
};

// Object Struct
extern ObjectGigaMetal *GigaMetal;

// Standard Entity Events
void GigaMetal_Update(void);
void GigaMetal_LateUpdate(void);
void GigaMetal_StaticUpdate(void);
void GigaMetal_Draw(void);
void GigaMetal_Create(void *data);
void GigaMetal_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void GigaMetal_EditorDraw(void);
void GigaMetal_EditorLoad(void);
#endif
void GigaMetal_Serialize(void);

// Extra Entity Functions
void GigaMetal_Draw_Cover(void);
void GigaMetal_Draw_Head(void);
void GigaMetal_Draw_Shoulder(void);
void GigaMetal_Draw_Arm(void);
void GigaMetal_Draw_LaserEdge(void);
void GigaMetal_Draw_Shard(void);

void GigaMetal_HandleCameraMovement(void);

void GigaMetal_CheckPlayerCollisions(void);
void GigaMetal_Hit(void);

void GigaMetal_StateBody_AwaitPlayer(void);
void GigaMetal_StateBody_SetupComponents(void);
void GigaMetal_StateBody_Transformed(void);
void GigaMetal_StateBody_Roar(void);
void GigaMetal_StateBody_Marching(void);
void GigaMetal_StateBody_Destroyed(void);

void GigaMetal_StateShoulder_Impact(void);

void GigaMetal_StateHead_Impact(void);

void GigaMetal_State_Laser(void);

void GigaMetal_StateLaser_Finish(void);

void GigaMetal_State_Shard(void);

void GigaMetal_StateHead_PrepareRoar(void);
void GigaMetal_StateHead_Roar(void);
void GigaMetal_StateHead_FinishRoar(void);
void GigaMetal_StateHead_PrepareLaser(void);
void GigaMetal_StateHead_ChargeLaser(void);
void GigaMetal_StateHead_FiringLaser(void);
void GigaMetal_StateHead_TargetingPlayer(void);
void GigaMetal_StateHead_ReleasingBombs(void);

void GigaMetal_HandleArmCollisions(void);
void GigaMetal_StateArm_Idle_Front(void);
void GigaMetal_StateArm_Idle_Behind(void);
void GigaMetal_StateArm_Marching(void);
#endif

#endif //! OBJ_GIGAMETAL_H
