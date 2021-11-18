#ifndef OBJ_TURRETSWITCH_H
#define OBJ_TURRETSWITCH_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxShot;
    Hitbox hitboxProjectile;
} ObjectTurretSwitch;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int32 type;
    bool32 walkOnto;
    uint8 tag;
    int32 field_64;
    int32 field_68;
    bool32 activated;
    int32 field_70;
    int32 field_74;
    int32 field_78;
    Animator animator;
    StateMachine(state);
    int32 timer;
    Vector2 startPos;
    Hitbox hitbox;
    Hitbox hitboxRange;
} EntityTurretSwitch;

// Object Struct
extern ObjectTurretSwitch *TurretSwitch;

// Standard Entity Events
void TurretSwitch_Update(void);
void TurretSwitch_LateUpdate(void);
void TurretSwitch_StaticUpdate(void);
void TurretSwitch_Draw(void);
void TurretSwitch_Create(void* data);
void TurretSwitch_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void TurretSwitch_EditorDraw(void);
void TurretSwitch_EditorLoad(void);
#endif
void TurretSwitch_Serialize(void);

// Extra Entity Functions
void TurretSwitch_CheckPlayerCollisions(void);
void TurretSwitch_Break(EntityTurretSwitch *self, EntityPlayer *player);

void TurretSwitch_State_Setup(void);
void TurretSwitch_State_Turret(void);
void TurretSwitch_State_Projectile(void);

#endif //!OBJ_TURRETSWITCH_H
