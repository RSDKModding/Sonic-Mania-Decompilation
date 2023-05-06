#ifndef OBJ_HOTARU_H
#define OBJ_HOTARU_H

#include "Game.h"

typedef enum {
    HOTARU_GOOD,
    HOTARU_BAD,
} HotaruQualities;

typedef enum {
    HOTARU_ATTACK_IDLE,
    HOTARU_ATTACK_CHARGING,
    HOTARU_ATTACK_ATTACKING,
} HotaruAttackStates;

// Object Class
struct ObjectHotaru {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitboxTrigger;
    int32 unused1;
    int32 unused2;
    Hitbox hitboxElectricity;
    uint16 aniFrames;
};

// Entity Class
struct EntityHotaru {
    RSDK_ENTITY
    StateMachine(state);
    uint8 quality;
    Vector2 dist;
    int32 distX1;
    int32 distX2;
    int32 speed;
    uint8 preset;
    Vector2 offset2;
    Vector2 offset1;
    uint8 hotaruDir;
    uint8 destroyedHotarus;
    Vector2 startPos;
    uint8 startDir;
    uint8 screenID;
    int32 timer;
    EntityPlayer *playerPtr;
    uint8 unused1;
    uint8 unused2;
    uint8 attackState;
    Vector2 electricityOffset;
    Animator mainAnimator;
    Animator bulbAnimator;
    Animator electricityAnimator;
};

// Object Struct
extern ObjectHotaru *Hotaru;

// Standard Entity Events
void Hotaru_Update(void);
void Hotaru_LateUpdate(void);
void Hotaru_StaticUpdate(void);
void Hotaru_Draw(void);
void Hotaru_Create(void *data);
void Hotaru_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Hotaru_EditorDraw(void);
void Hotaru_EditorLoad(void);
#endif
void Hotaru_Serialize(void);

// Extra Entity Functions
void Hotaru_DebugSpawn(void);
void Hotaru_DebugDraw(void);

void Hotaru_CheckPlayerCollisions(void);
void Hotaru_CheckOffScreen(void);

void Hotaru_State_Init(void);
void Hotaru_State_CheckPlayerInRange(void);
void Hotaru_State_FoundPlayer(void);
void Hotaru_State_FlyOnScreen(void);
void Hotaru_State_AttachedToScreen(void);
void Hotaru_State_Charging(void);
void Hotaru_State_Attacking(void);
void Hotaru_State_FinishedAttacking(void);

#endif //! OBJ_HOTARU_H
