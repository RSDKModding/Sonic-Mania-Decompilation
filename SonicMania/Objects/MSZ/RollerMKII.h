#ifndef OBJ_ROLLERMKII_H
#define OBJ_ROLLERMKII_H

#include "Game.h"

// Object Class
struct ObjectRollerMKII {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitboxObject;
    Hitbox hitboxInner_Rolling;
    Hitbox hitboxOuter_Rolling;
    Hitbox hitboxInner_Idle;
    Hitbox hitboxOuter_Idle;
    uint16 aniFrames;
    uint16 sfxBumper;
    uint16 sfxJump;
    uint16 sfxDropDash;
    uint16 sfxRelease;
    uint16 sfxSkidding;
};

// Entity Class
struct EntityRollerMKII {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 startPos;
    uint8 startDir;
    uint8 timer;
    bool32 touchedGround;
    EntityPlayer *playerPtr;
    Animator animator;
};

// Object Struct
extern ObjectRollerMKII *RollerMKII;

// Standard Entity Events
void RollerMKII_Update(void);
void RollerMKII_LateUpdate(void);
void RollerMKII_StaticUpdate(void);
void RollerMKII_Draw(void);
void RollerMKII_Create(void *data);
void RollerMKII_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void RollerMKII_EditorDraw(void);
void RollerMKII_EditorLoad(void);
#endif
void RollerMKII_Serialize(void);

// Extra Entity Functions
void RollerMKII_DebugSpawn(void);
void RollerMKII_DebugDraw(void);

void RollerMKII_CheckOffScreen(void);

void RollerMKII_CheckPlayerCollisions(void);
void RollerMKII_CheckPlayerCollisions_Rolling(void);
int32 RollerMKII_HandleObjectCollisions(Entity *otherEntity, Hitbox *hitbox);
bool32 RollerMKII_HandlePlatformCollisions(EntityPlatform *platform);
void RollerMKII_HandleCollisions(void);

void RollerMKII_State_Init(void);
void RollerMKII_State_Idle(void);
void RollerMKII_State_SpinUp(void);
void RollerMKII_State_RollDelay(void);
void RollerMKII_State_Rolling_Air(void);
void RollerMKII_State_Rolling_Ground(void);
void RollerMKII_State_Bumped(void);

#endif //! OBJ_ROLLERMKII_H
