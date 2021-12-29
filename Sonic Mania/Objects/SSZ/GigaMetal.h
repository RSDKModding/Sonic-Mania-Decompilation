#ifndef OBJ_GIGAMETAL_H
#define OBJ_GIGAMETAL_H

#include "SonicMania.h"

#if RETRO_USE_PLUS

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
}GigaMetalAnims;

// Object Class
struct ObjectGigaMetal {
    RSDK_OBJECT
    int32 invincibleTimer;
    int32 field_8;
    uint16 aniFrames;
    uint16 sfxRoar;
    uint16 sfxImpact;
    uint16 sfxTarget;
    uint16 sfxPimpom;
    uint16 sfxCannon;
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    Hitbox hitbox4;
};

// Entity Class
struct EntityGigaMetal {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    Entity *body;
    Entity *head;
    Entity *ruby;
    Entity *cover;
    Entity *shoulder;
    Entity *frontArm;
    Entity *backArm;
    Entity *metalSonic;
    int32 timer;
    int32 health;
    int32 field_88;
    Vector2 field_8C;
    Vector2 field_94;
    Animator animator;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    uint8 aniID;
    uint8 frameID;
    int32 field_100;
    int32 field_104;
    int32 field_108;
    int32 field_10C;
};

// Object Struct
extern ObjectGigaMetal *GigaMetal;

// Standard Entity Events
void GigaMetal_Update(void);
void GigaMetal_LateUpdate(void);
void GigaMetal_StaticUpdate(void);
void GigaMetal_Draw(void);
void GigaMetal_Create(void* data);
void GigaMetal_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void GigaMetal_EditorDraw(void);
void GigaMetal_EditorLoad(void);
#endif
void GigaMetal_Serialize(void);

// Extra Entity Functions
void GigaMetal_StateDraw3_Unknown(void);
void GigaMetal_StateDraw0_Unknown(void);
void GigaMetal_StateDraw1_Unknown(void);
void GigaMetal_StateDraw6_Unknown(void);
void GigaMetal_StateDraw8_Unknown(void);
void GigaMetal_StateDraw10_Unknown(void);

void GigaMetal_HandleCameraMovement(void);

void GigaMetal_CheckPlayerCollisions(void);
void GigaMetal_Hit(void);

void GigaMetal_State_SetupBounds(void);
void GigaMetal_State_Unknown2(void);
void GigaMetal_State_Unknown3(void);
void GigaMetal_State_Unknown4(void);
void GigaMetal_State_Unknown5(void);
void GigaMetal_State_Destroyed(void);

void GigaMetal_StateShoulder_Unknown1(void);

void GigaMetal_StateHead_Unknown3(void);

void GigaMetal_State8_Unknown(void);

void GigaMetal_StateHead_Unknown10(void);

void GigaMetal_State10_Unknown(void);

void GigaMetal_StateHead_Unknown1(void);
void GigaMetal_StateHead_Unknown2(void);
void GigaMetal_StateHead_Unknown7(void);
void GigaMetal_StateHead_Unknown4(void);
void GigaMetal_StateHead_Unknown5(void);
void GigaMetal_StateHead_Unknown6(void);
void GigaMetal_StateHead_Unknown8(void);
void GigaMetal_StateHead_Unknown9(void);

void GigaMetal_HandleArmCollisions(void);
void GigaMetal_StateArm_Unknown1(void);
void GigaMetal_StateArm_Unknown2(void);
void GigaMetal_StateArm_Unknown3(void);

#endif

#endif //!OBJ_GIGAMETAL_H
