#ifndef OBJ_ICEBOMBA_H
#define OBJ_ICEBOMBA_H

#include "Game.h"

// Object Class
struct ObjectIceBomba {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitboxBomb;
    Hitbox hitboxRange;
    uint16 aniFrames;
    uint16 sfxExplosion;
    uint16 sfxFreeze;
};

// Entity Class
struct EntityIceBomba {
    RSDK_ENTITY
    StateMachine(state);
    uint8 dir;
    uint16 dist;
    uint16 dip;
    uint8 startDir;
    Vector2 startPos;
    uint32 startDist;
    Animator bodyAnimator;
    Animator wingAnimator;
    Animator bombAnimator;
};

// Object Struct
extern ObjectIceBomba *IceBomba;

// Standard Entity Events
void IceBomba_Update(void);
void IceBomba_LateUpdate(void);
void IceBomba_StaticUpdate(void);
void IceBomba_Draw(void);
void IceBomba_Create(void *data);
void IceBomba_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void IceBomba_EditorDraw(void);
void IceBomba_EditorLoad(void);
#endif
void IceBomba_Serialize(void);

// Extra Entity Functions
void IceBomba_DebugDraw(void);
void IceBomba_DebugSpawn(void);

void IceBomba_CheckOffScreen(void);

void IceBomba_HandlePlayerCollisions(void);

void IceBomba_State_Init(void);
void IceBomba_State_Flying(void);
void IceBomba_State_Turning(void);
void IceBomba_State_FlyAway(void);

void IceBomba_State_Bomb(void);

#endif //! OBJ_ICEBOMBA_H
