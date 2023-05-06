#ifndef OBJ_ERZGUNNER_H
#define OBJ_ERZGUNNER_H

#include "Game.h"

typedef enum {
    ERZGUNNER_BOSS,
    ERZGUNNER_LAUNCHROCKET,
    ERZGUNNER_MORTAR,
    ERZGUNNER_NAPALM,
    ERZGUNNER_DUD,
    ERZGUNNER_NAPALM_EXPLOSION,
    ERZGUNNER_MORTAR_EXPLOSION,
} ERZGunnerTypes;

// Object Class
struct ObjectERZGunner {
    RSDK_OBJECT
    // Technically a "Vector2" but since that can't be saved in static objects, it's an int32 array
    TABLE(int32 rocketOffsets[32], { -22, -24, -10, -24, -22, -31, -10, -31, -22, -38, -10, -38, -22, -45, -10, -45,
                                     10,  -24, 22,  -24, 10,  -31, 22,  -31, 10,  -38, 22,  -38, 10,  -45, 22,  -45 });
    int32 launchedRocketID;
    Hitbox hitboxNapalm;
    Hitbox hitboxMortar;
    Hitbox hitboxDud;
    uint16 aniFrames;
};

// Entity Class
struct EntityERZGunner {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    Vector2 originPos;
    int32 type;
    EntityERZGunner *parent;
    int32 timer;
    int32 invincibilityTimer;
    int32 fireAnimTimer;
    int32 rocketLaunchCount;
    int32 rocketOffsetID;
    int32 napalmExplosionPos;
    Vector2 screenPos;
    Animator mainAnimator;
    Animator fxAnimator;
    Animator tailAnimator;
    Animator parachuteAnimator;
    Animator unusedAnimator;
};

// Object Struct
extern ObjectERZGunner *ERZGunner;

// Standard Entity Events
void ERZGunner_Update(void);
void ERZGunner_LateUpdate(void);
void ERZGunner_StaticUpdate(void);
void ERZGunner_Draw(void);
void ERZGunner_Create(void *data);
void ERZGunner_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void ERZGunner_EditorDraw(void);
void ERZGunner_EditorLoad(void);
#endif
void ERZGunner_Serialize(void);

// Extra Entity Functions
void ERZGunner_HandleDudExhaust(void);
void ERZGunner_HandleMalfunctionDudExhaust(void);
void ERZGunner_SpawnDust(void);
void ERZGunner_HandleRotations(int32 angle);
void ERZGunner_CheckPlayerMissileCollisions(void);
void ERZGunner_CheckPlayerExplosionCollisions(void);
void ERZGunner_Hit(EntityERZGunner *entity);

void ERZGunner_Draw_Gunner(void);
void ERZGunner_Draw_RocketLaunch(void);
void ERZGunner_Draw_Rocket(void);

void ERZGunner_State_Idle(void);
void ERZGunner_State_LaunchRockets(void);

void ERZGunner_State_LaunchedRocket(void);
void ERZGunner_State_Mortar(void);
void ERZGunner_State_Napalm(void);
void ERZGunner_State_Dud_Active(void);
void ERZGunner_State_Dud_HitByPlayer(void);
void ERZGunner_State_Dud_Malfunction(void);
void ERZGunner_State_Dud_Explode(void);

void ERZGunner_State_NapalmExplosion(void);
void ERZGunner_State_MortarExplosion(void);

#endif //! OBJ_ERZGUNNER_H
