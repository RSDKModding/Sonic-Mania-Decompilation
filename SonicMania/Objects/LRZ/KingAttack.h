#ifndef OBJ_KINGATTACK_H
#define OBJ_KINGATTACK_H

#include "Game.h"

typedef enum {
    KINGATTACK_LASER,
    KINGATTACK_LASERBLAST,
    KINGATTACK_ORBIT,
    KINGATTACK_TRAIL,
    KINGATTACK_LARGEBULLET,
    KINGATTACK_ENERGYLINE,
    KINGATTACK_SMALLBULLET,
} KingAttackTypes;

// Object Class
struct ObjectKingAttack {
    RSDK_OBJECT
    TABLE(int32 laserColors[8], { 0x000020, 0x000020, 0xE850D8, 0xE850D8, 0xE850D8, 0xE850D8, 0x000020, 0x000020 });
    uint16 aniFrames;
    uint16 sfxLaserSweep;
    uint16 sfxLaserErupt;
    uint16 sfxElecPulse;
    uint16 sfxTwinShot;
    bool32 laserEruptActive;
};

// Entity Class
struct EntityKingAttack {
    RSDK_ENTITY
    StateMachine(state);
    int32 type;
    int32 timer;
    Entity *parent;
    Vector2 targetPos;
    Vector2 targetVelocity;
    Vector2 laserVertPostions[8];
    int32 *laserColors;
    Hitbox hitbox;
    Animator animator;
};

// Object Struct
extern ObjectKingAttack *KingAttack;

// Standard Entity Events
void KingAttack_Update(void);
void KingAttack_LateUpdate(void);
void KingAttack_StaticUpdate(void);
void KingAttack_Draw(void);
void KingAttack_Create(void *data);
void KingAttack_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void KingAttack_EditorDraw(void);
void KingAttack_EditorLoad(void);
#endif
void KingAttack_Serialize(void);

// Extra Entity Functions
bool32 KingAttack_SfxCheck_ElecIdle(void);
void KingAttack_CheckPlayerCollisions(void);
void KingAttack_HandleLaserPositions(void);

void KingAttack_State_Laser(void);
void KingAttack_State_LaserBlast_Delay(void);
void KingAttack_State_LaserBlast_Erupt(void);
void KingAttack_State_OrbitAppear(void);
void KingAttack_State_Orbiting(void);
void KingAttack_State_OrbitLaunched(void);
void KingAttack_State_Trail(void);
void KingAttack_State_LargeBullet_Appear(void);
void KingAttack_State_EnergyLine(void);
void KingAttack_State_LargeBullet_TwinShot(void);
void KingAttack_State_SmallBullet(void);
void KingAttack_State_SmallBullet_Impact(void);

#endif //! OBJ_KINGATTACK_H
