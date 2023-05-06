#ifndef OBJ_DANGO_H
#define OBJ_DANGO_H

#include "Game.h"

// Object Class
struct ObjectDango {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitboxCurlRange;
    uint16 aniFrames;
    uint16 sfxBumper;
};

// Entity Class
struct EntityDango {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 rollDelay;
    bool32 preventStateChange;
    Vector2 startPos;
    uint8 startDir;
    Animator animator;
};

// Object Struct
extern ObjectDango *Dango;

// Standard Entity Events
void Dango_Update(void);
void Dango_LateUpdate(void);
void Dango_StaticUpdate(void);
void Dango_Draw(void);
void Dango_Create(void *data);
void Dango_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Dango_EditorDraw(void);
void Dango_EditorLoad(void);
#endif
void Dango_Serialize(void);

// Extra Entity Functions
void Dango_DebugSpawn(void);
void Dango_DebugDraw(void);

void Dango_CheckOffScreen(void);
void Dango_CheckPlayerCollisions(void);
bool32 Dango_HandleMovement(StateMachine(nextState), uint8 anim);

void Dango_State_Init(void);
void Dango_State_Walking(void);
void Dango_State_Turning(void);
void Dango_State_Falling_Uncurled(void);
void Dango_State_Curling(void);
void Dango_State_Rolling(void);
void Dango_State_Falling_Curled(void);
void Dango_State_Uncurling(void);

#if MANIA_USE_PLUS
void Dango_StateTaunt_Setup(void);
void Dango_StateTaunt_RollIn(void);
void Dango_StateTaunt_KnockedRuby(void);
void Dango_StateTaunt_Taunting(void);
void Dango_StateTaunt_Turning(void);
void Dango_StateTaunt_RollOut(void);
#endif

#endif //! OBJ_DANGO_H
