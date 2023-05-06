#ifndef OBJ_PKINGATTACK_H
#define OBJ_PKINGATTACK_H

#include "Game.h"

typedef enum {
    PKINGATTACK_LASER, // Leftover from LRZ/KingAttack, not used here
    PKINGATTACK_ORBIT,
    PKINGATTACK_LAUNCHED,
    PKINGATTACK_TRAIL,
    PKINGATTACK_LARGEBULLET, // Leftover from LRZ/KingAttack, not used here
    PKINGATTACK_ENERGYLINE,  // Leftover from LRZ/KingAttack, not used here
    PKINGATTACK_SMALLBULLET, // Leftover from LRZ/KingAttack, not used here, though (new) code for it does exist
} PKingAttackTypes;

// Object Class
struct ObjectPKingAttack {
    RSDK_OBJECT
    // Not Used because he doesn't fire lasers... this object was prolly built off LRZ/KingAttack tbh
    TABLE(int32 laserColors[8], { 0x000020, 0x000020, 0xE850D8, 0xE850D8, 0xE850D8, 0xE850D8, 0x000020, 0x000020 });
    uint16 sfxPulse;
    uint16 aniFrames;
};

// Entity Class
struct EntityPKingAttack {
    RSDK_ENTITY
    StateMachine(state);
    int32 type;
    int32 timer;
    Entity *target;
    Vector2 targetPos;
    Vector2 targetVelocity;
    Vector2 laserVertPostions[8]; // Leftover from LRZ/KingAttack, not used here
    int32 *laserColors;           // Leftover from LRZ/KingAttack, not used here
    Hitbox hitbox;
    Animator animator;
};

// Object Struct
extern ObjectPKingAttack *PKingAttack;

// Standard Entity Events
void PKingAttack_Update(void);
void PKingAttack_LateUpdate(void);
void PKingAttack_StaticUpdate(void);
void PKingAttack_Draw(void);
void PKingAttack_Create(void *data);
void PKingAttack_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PKingAttack_EditorDraw(void);
void PKingAttack_EditorLoad(void);
#endif
void PKingAttack_Serialize(void);

// Extra Entity Functions
void PKingAttack_CheckPlayerCollisions(void);

void PKingAttack_State_OrbitAppear(void);
void PKingAttack_State_Orbiting(void);
void PKingAttack_State_OrbitLaunched(void);
void PKingAttack_State_Trail(void);
void PKingAttack_State_SmallBullet(void);

#endif //! OBJ_PKINGATTACK_H
