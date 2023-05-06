#ifndef OBJ_SPEEDBOOSTER_H
#define OBJ_SPEEDBOOSTER_H

#include "Game.h"

// Object Class
struct ObjectSpeedBooster {
    RSDK_OBJECT
    StateMachine(defaultState);
    Hitbox hitbox;
    uint16 aniFrames;
    uint16 sfxSpeedBooster;
};

// Entity Class
struct EntitySpeedBooster {
    RSDK_ENTITY
    StateMachine(state);
    uint8 speed;
    int32 playerPos[PLAYER_COUNT];
    Vector2 drawPos;
    uint8 timer;
    uint8 playerTimer[PLAYER_COUNT];
    Animator animator;
};

// Object Struct
extern ObjectSpeedBooster *SpeedBooster;

// Standard Entity Events
void SpeedBooster_Update(void);
void SpeedBooster_LateUpdate(void);
void SpeedBooster_StaticUpdate(void);
void SpeedBooster_Draw(void);
void SpeedBooster_Create(void *data);
void SpeedBooster_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void SpeedBooster_EditorDraw(void);
void SpeedBooster_EditorLoad(void);
#endif
void SpeedBooster_Serialize(void);

void SpeedBooster_DebugSpawn(void);
void SpeedBooster_DebugDraw(void);

// Extra Entity Functions
void SpeedBooster_State_SpeedBooster(void);
void SpeedBooster_HandleInteractions(void);

void SpeedBooster_State_SSZFire(void);
void SpeedBooster_State_SSZRecoil(void);
void SpeedBooster_State_SSZRetract(void);

void SpeedBooster_State_SSZBullet(void);

#endif //! OBJ_SPEEDBOOSTER_H
