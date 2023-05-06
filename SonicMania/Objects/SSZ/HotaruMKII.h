#ifndef OBJ_HOTARUMKII_H
#define OBJ_HOTARUMKII_H

#include "Game.h"

typedef enum {
    HOTARUMKII_MAIN,
    HOTARUMKII_FLASH,
    HOTARUMKII_LASER,
} HotaruMKIITypes;

// Object Class
struct ObjectHotaruMKII {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitboxLaser;
    uint16 aniFrames;
    uint16 sfxAppear;
    uint16 sfxLaser;
    uint16 sfxFly;
    uint16 sfxCharge;
};

// Entity Class
struct EntityHotaruMKII {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 startPos;
    uint8 startDir;
    int32 timer;
    Vector2 moveAcceleration;
    uint8 unused1;
    uint8 unused2;
    uint8 childCount;
    Vector2 curOffset;
    uint8 origin;
    Vector2 offset1;
    Vector2 offset2;
    Vector2 offset3;
    int32 offsetID;
    EntityPlayer *playerPtr;
    int32 type;
    Vector2 triggerSize;
    Hitbox hitboxTrigger;
    Animator mainAnimator;
    Animator flashAnimator;
    Animator unusedAnimator;
};

// Object Struct
extern ObjectHotaruMKII *HotaruMKII;

// Standard Entity Events
void HotaruMKII_Update(void);
void HotaruMKII_LateUpdate(void);
void HotaruMKII_StaticUpdate(void);
void HotaruMKII_Draw(void);
void HotaruMKII_Create(void *data);
void HotaruMKII_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void HotaruMKII_EditorDraw(void);
void HotaruMKII_EditorLoad(void);
#endif
void HotaruMKII_Serialize(void);

// Extra Entity Functions
void HotaruMKII_DebugSpawn(void);
void HotaruMKII_DebugDraw(void);

void HotaruMKII_CheckPlayerCollisions(void);
void HotaruMKII_CheckOffScreen(void);
void HotaruMKII_HandleDistances(EntityPlayer *player);

void HotaruMKII_State_Init(void);
void HotaruMKII_State_CheckPlayerInRange(void);
void HotaruMKII_State_FlyAway(void);
void HotaruMKII_State_FlyOnScreen(void);
void HotaruMKII_State_AttackDelay(void);
void HotaruMKII_State_Charging(void);
void HotaruMKII_State_LaserAttack(void);

void HotaruMKII_State_Flash(void);

void HotaruMKII_State_Laser(void);
void HotaruMKII_State_LaserStrike(void);

#endif //! OBJ_HOTARUMKII_H
