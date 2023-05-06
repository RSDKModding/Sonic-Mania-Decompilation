#ifndef OBJ_CLAPPERBOARD_H
#define OBJ_CLAPPERBOARD_H

#include "Game.h"

// Object Class
struct ObjectClapperboard {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxWhack;
};

// Entity Class
struct EntityClapperboard {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateCollide);
    uint8 takeID;
    int32 clapSpeed;
    int32 stoodPlayers;
    Hitbox hitbox;
    Vector2 amplitudeL;
    Vector2 amplitudeR;
    Animator clapperTopAnimator;
    Animator clapperBottomAnimator;
    Animator boardAnimator;
    Animator numbersAnimator;
};

// Object Struct
extern ObjectClapperboard *Clapperboard;

// Standard Entity Events
void Clapperboard_Update(void);
void Clapperboard_LateUpdate(void);
void Clapperboard_StaticUpdate(void);
void Clapperboard_Draw(void);
void Clapperboard_Create(void *data);
void Clapperboard_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Clapperboard_EditorDraw(void);
void Clapperboard_EditorLoad(void);
#endif
void Clapperboard_Serialize(void);

// Extra Entity Functions
void Clapperboard_Collide_Left(void);
void Clapperboard_Collide_Right(void);

void Clapperboard_State_Idle(void);
void Clapperboard_State_ClappingL(void);
void Clapperboard_State_ClapReboundL(void);
void Clapperboard_State_ClappingR(void);
void Clapperboard_State_ClapReboundR(void);

#endif //! OBJ_CLAPPERBOARD_H
