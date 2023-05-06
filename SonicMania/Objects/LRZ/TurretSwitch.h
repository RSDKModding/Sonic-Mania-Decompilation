#ifndef OBJ_TURRETSWITCH_H
#define OBJ_TURRETSWITCH_H

#include "Game.h"

// Object Class
struct ObjectTurretSwitch {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxShot;
    Hitbox hitboxProjectile;
};

// Entity Class
struct EntityTurretSwitch {
    MANIA_BUTTON_BASE

    Animator animator;
    StateMachine(state);
    int32 timer;
    Vector2 startPos;
    Hitbox hitbox;
    Hitbox hitboxRange;
};

// Object Struct
extern ObjectTurretSwitch *TurretSwitch;

// Standard Entity Events
void TurretSwitch_Update(void);
void TurretSwitch_LateUpdate(void);
void TurretSwitch_StaticUpdate(void);
void TurretSwitch_Draw(void);
void TurretSwitch_Create(void *data);
void TurretSwitch_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void TurretSwitch_EditorDraw(void);
void TurretSwitch_EditorLoad(void);
#endif
void TurretSwitch_Serialize(void);

// Extra Entity Functions
void TurretSwitch_CheckPlayerCollisions(void);
void TurretSwitch_Break(EntityTurretSwitch *turret, EntityPlayer *player);

void TurretSwitch_State_Init(void);
void TurretSwitch_State_Turret(void);
void TurretSwitch_State_Projectile(void);

#endif //! OBJ_TURRETSWITCH_H
