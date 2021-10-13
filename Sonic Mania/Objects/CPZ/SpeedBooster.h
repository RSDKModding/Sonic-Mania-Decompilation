#ifndef OBJ_SPEEDBOOSTER_H
#define OBJ_SPEEDBOOSTER_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
	void (*defaultState)(void);
	Hitbox hitbox;
    uint16 animID;
	uint16 sfxID;
} ObjectSpeedBooster;

// Entity Class
typedef struct {
	RSDK_ENTITY
	void (*state)(void);
	uint8 speed;
	int32 playerPos[4];
	Vector2 drawPos;
	uint8 cooldown;
	uint8 playerCooldown[4];
	Animator animator;
} EntitySpeedBooster;

// Object Struct
extern ObjectSpeedBooster *SpeedBooster;

// Standard Entity Events
void SpeedBooster_Update(void);
void SpeedBooster_LateUpdate(void);
void SpeedBooster_StaticUpdate(void);
void SpeedBooster_Draw(void);
void SpeedBooster_Create(void* data);
void SpeedBooster_StageLoad(void);
void SpeedBooster_EditorDraw(void);
void SpeedBooster_EditorLoad(void);
void SpeedBooster_Serialize(void);

void SpeedBooster_DebugSpawn(void);
void SpeedBooster_DebugDraw(void);

// Extra Entity Functions
void SpeedBooster_BasicState(void);
void SpeedBooster_SSZState(void);
void SpeedBooster_MovingState(void);

void SpeedBooster_Interact(void);

void SpeedBooster_Wait1(void);
void SpeedBooster_Wait2(void);

#endif //!OBJ_SPEEDBOOSTER_H
