#ifndef OBJ_SPEEDBOOSTER_H
#define OBJ_SPEEDBOOSTER_H

#include "SonicMania.h"

// Object Class
struct ObjectSpeedBooster {
    RSDK_OBJECT
    StateMachine(defaultState);
	Hitbox hitbox;
    uint16 animID;
	uint16 sfxID;
};

// Entity Class
struct EntitySpeedBooster {
    RSDK_ENTITY
    StateMachine(state);
    uint8 speed;
    int32 playerPos[PLAYER_MAX];
    Vector2 drawPos;
    uint8 cooldown;
    uint8 playerCooldown[PLAYER_MAX];
    Animator animator;
};

// Object Struct
extern ObjectSpeedBooster *SpeedBooster;

// Standard Entity Events
void SpeedBooster_Update(void);
void SpeedBooster_LateUpdate(void);
void SpeedBooster_StaticUpdate(void);
void SpeedBooster_Draw(void);
void SpeedBooster_Create(void* data);
void SpeedBooster_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void SpeedBooster_EditorDraw(void);
void SpeedBooster_EditorLoad(void);
#endif
void SpeedBooster_Serialize(void);

void SpeedBooster_DebugSpawn(void);
void SpeedBooster_DebugDraw(void);

// Extra Entity Functions
void SpeedBooster_State_SpeedBooster(void);
void SpeedBooster_Interact(void);

void SpeedBooster_State_SSZFire(void);
void SpeedBooster_State_SSZRecoil(void);
void SpeedBooster_State_SSZRetract(void);

void SpeedBooster_State_SSZBullet(void);

#endif //!OBJ_SPEEDBOOSTER_H
