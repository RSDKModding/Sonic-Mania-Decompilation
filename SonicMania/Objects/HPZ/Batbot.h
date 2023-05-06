#ifndef OBJ_BATBOT_H
#define OBJ_BATBOT_H

#include "Game.h"

// Object Class
struct ObjectBatbot {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitboxSpinCheck;
    Hitbox hitboxAttack;
    Hitbox hitboxPlayer;
    uint16 aniFrames;
    uint8 directionTable[5];
};

// Entity Class
struct EntityBatbot {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 startPos;
    uint8 startDir;
    uint16 timer;
    int32 arcAngle;
    int32 originY;
    Vector2 landPos;
    int32 unused1;
    int32 unused2;
    int32 swoopAngle;
    EntityPlayer *playerPtr;
    uint8 swoopDir;
    bool32 showJet;
    uint16 unused3; // set to 0, not used, value isn't in S2
    Animator bodyAnimator;
    Animator jetAnimator;
};

// Object Entity
extern ObjectBatbot *Batbot;

// Standard Entity Events
void Batbot_Update(void);
void Batbot_LateUpdate(void);
void Batbot_StaticUpdate(void);
void Batbot_Draw(void);
void Batbot_Create(void *data);
void Batbot_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Batbot_EditorDraw(void);
void Batbot_EditorLoad(void);
#endif
void Batbot_Serialize(void);

// Extra Entity Functions
void Batbot_DebugSpawn(void);
void Batbot_DebugDraw(void);

void Batbot_CheckPlayerCollisions(void);

void Batbot_State_Init(void);
void Batbot_State_Idle(void);
void Batbot_State_Attack(void);
void Batbot_State_SwoopLeft(void);
void Batbot_State_SwoopRight(void);

#endif //! OBJ_BATBOT_H
