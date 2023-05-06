#ifndef OBJ_DCEVENT_H
#define OBJ_DCEVENT_H

#include "Game.h"

typedef enum {
    DCEVENT_EGGMAN,
    DCEVENT_BOMB,
    DCEVENT_BUBBLE,
} DCEventTypes;

typedef enum {
    DCEVENT_EGGMAN_PLACEBOMB,
    DCEVENT_EGGMAN_SWIMMING,
} DCEventEggmanTypes;

// Object Class
struct ObjectDCEvent {
    RSDK_OBJECT
    Hitbox unusedHitbox1;
    Hitbox hitboxBomb;
    bool32 canExplodeBombs;
    uint16 aniFrames;
    uint16 eggmanFrames;
    uint16 sfxExplosion;
    uint16 sfxRumble;
    uint16 sfxImpact6;
    uint16 sfxImpact4;
};

// Entity Class
struct EntityDCEvent {
    RSDK_ENTITY
    StateMachine(state);
    uint8 numChildren;
    uint8 type;
    int32 remainingBombs;
    int32 startY;
    int32 timer;
    Animator animator;
};

// Object Struct
extern ObjectDCEvent *DCEvent;

// Standard Entity Events
void DCEvent_Update(void);
void DCEvent_LateUpdate(void);
void DCEvent_StaticUpdate(void);
void DCEvent_Draw(void);
void DCEvent_Create(void *data);
void DCEvent_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void DCEvent_EditorDraw(void);
void DCEvent_EditorLoad(void);
#endif
void DCEvent_Serialize(void);

// Extra Entity Functions
void DCEvent_State_Collapse(void);
void DCEvent_StateEggmanBomber_AwaitPlayer(void);

void DCEvent_Input_MoveRight(void);
void DCEvent_Input_LookDown(void);

void DCEvent_StateEggmanBomber_WaitForLookDown(void);
void DCEvent_StateEggmanBomber_Swimming(void);
void DCEvent_StateEggmanBomber_PlaceBomb(void);
void DCEvent_StateEggmanBomber_PlacedAllBombs(void);

void DCEvent_StateEggmanSwim_AwaitPlayer(void);
void DCEvent_StateEggmanSwim_Swimming(void);

void DCEvent_State_Bomb(void);
void DCEvent_State_BombExplode(void);

#endif //! OBJ_DCEVENT_H
