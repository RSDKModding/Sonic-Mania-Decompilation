#ifndef OBJ_PHANTOMGUNNER_H
#define OBJ_PHANTOMGUNNER_H

#include "Game.h"

typedef enum {
    PHANTOMGUNNER_BOSS,
    PHANTOMGUNNER_LAUNCHROCKET,
    PHANTOMGUNNER_MORTAR,
    PHANTOMGUNNER_NAPALM,
    PHANTOMGUNNER_DUD,
    PHANTOMGUNNER_NAPALM_EXPLOSION,
    PHANTOMGUNNER_MORTAR_EXPLOSION,
} PhantomGunnerTypes;

// Object Class
struct ObjectPhantomGunner {
    RSDK_OBJECT
    // Technically a "Vector2" but since that can't be saved in static objects, it's an int array
    TABLE(int32 rocketOffsets[0x20], { -22, -24, -10, -24, -22, -31, -10, -31, -22, -38, -10, -38, -22, -45, -10, -45,
                                       10,  -24, 22,  -24, 10,  -31, 22,  -31, 10,  -38, 22,  -38, 10,  -45, 22,  -45 });
    int32 launchedRocketID;
    Hitbox hitboxNapalm;
    Hitbox hitboxMortar;
    Hitbox hitboxDud;
    uint16 aniFrames;
    uint16 sfxCannonFire;
};

// Entity Class
struct EntityPhantomGunner {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    Vector2 originPos;
    Vector2 startPos;
    int32 type;
    EntityPhantomGunner *parent;
    int32 timer;
    int32 invincibilityTimer;
    int32 unused;
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
extern ObjectPhantomGunner *PhantomGunner;

// Standard Entity Events
void PhantomGunner_Update(void);
void PhantomGunner_LateUpdate(void);
void PhantomGunner_StaticUpdate(void);
void PhantomGunner_Draw(void);
void PhantomGunner_Create(void *data);
void PhantomGunner_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PhantomGunner_EditorDraw(void);
void PhantomGunner_EditorLoad(void);
#endif
void PhantomGunner_Serialize(void);

// Extra Entity Functions
void PhantomGunner_HandleDudExhaust(void);
void PhantomGunner_HandleMalfunctionDudExhaust(void);
void PhantomGunner_SpawnDust(void);
void PhantomGunner_HandleRotations(int32 angle);
void PhantomGunner_CheckPlayerMissileCollisions(void);
void PhantomGunner_CheckPlayerExplosionCollisions(void);
void PhantomGunner_Hit(EntityPhantomGunner *entity);

void PhantomGunner_Draw_Gunner(void);
void PhantomGunner_Draw_RocketLaunch(void);
void PhantomGunner_Draw_Rocket(void);

void PhantomGunner_State_ResetState(void);
void PhantomGunner_State_Idle(void);
void PhantomGunner_State_LaunchRockets(void);

void PhantomGunner_State_LaunchedRocket(void);
void PhantomGunner_State_Mortar(void);
void PhantomGunner_State_Napalm(void);
void PhantomGunner_State_Dud_Active(void);
void PhantomGunner_State_Dud_HitByPlayer(void);
void PhantomGunner_State_Dud_Malfunction(void);
void PhantomGunner_State_Dud_Explode(void);

void PhantomGunner_State_NapalmExplosion(void);
void PhantomGunner_State_MortarExplosion(void);

#endif //! OBJ_PHANTOMGUNNER_H
