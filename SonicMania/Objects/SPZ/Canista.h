#ifndef OBJ_CANISTA_H
#define OBJ_CANISTA_H

#include "Game.h"

typedef enum {
    CANISTA_TRIGGER_STATIC,
    CANISTA_TRIGGER_DETECT,
} CanistaTriggerModes;

// Object Class
struct ObjectCanista {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitboxRange;
    Hitbox hitboxDetect;
    Hitbox hitboxProjectile;
    uint16 aniFrames;
    uint16 sfxPon;
};

// Entity Class
struct EntityCanista {
    RSDK_ENTITY
    StateMachine(state);
    uint8 triggerMode;
    Vector2 detectOffset;
    Vector2 detectSize;
    int32 shootOffset;
    int32 shootSize;
    int32 stopTimer;
    int32 detectDelay;
    EntityPlayer *detectedPlayer;
    Vector2 startPos;
    uint8 startDir;
    uint8 moveDir;
    uint8 timer;
    Hitbox hitboxDetect;
    int32 unused1;
    int32 unused2;
    Animator mainAnimator;
    Animator tapeAnimator;
    Animator cannonAnimator;
};

// Object Struct
extern ObjectCanista *Canista;

// Standard Entity Events
void Canista_Update(void);
void Canista_LateUpdate(void);
void Canista_StaticUpdate(void);
void Canista_Draw(void);
void Canista_Create(void *data);
void Canista_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Canista_EditorDraw(void);
void Canista_EditorLoad(void);
#endif
void Canista_Serialize(void);

// Extra Entity Functions
void Canista_DebugSpawn(void);
void Canista_DebugDraw(void);

void Canista_CheckPlayerBadnikCollisions(void);
void Canista_CheckOffScreen(void);

void Canista_State_Init(void);
void Canista_State_Moving(void);
void Canista_State_Idle(void);

void Canista_CheckPlayerProjectileCollisions(void);
void Canista_StateProjectile_Shot(void);
void Canista_StateProjectile_Fall(void);

#endif //! OBJ_CANISTA_H
