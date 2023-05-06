#ifndef OBJ_PRESS_H
#define OBJ_PRESS_H

#include "Game.h"

// Object Class
struct ObjectPress {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitbox;
    uint16 sfxImpact;
    uint16 sfxPress;
#if MANIA_USE_PLUS
    bool32 canSuper;
#endif
};

// Entity Class
struct EntityPress {
    RSDK_ENTITY
    StateMachine(state);
    uint16 size;
    uint32 speed;
    int32 offTop;
    int32 offBottom;
    int32 topOffset;
    uint32 threads;
    int32 height;
    int32 timerStart;
    int32 timer;
    Vector2 drawPos;
    uint16 threadSprY;
    uint8 stoodPlayersRoof;
    uint8 stoodPlayersFloor;
    Animator crusherAnimator;
    Animator threadAnimator;
    Animator bumperAnimator;
};

// Object Struct
extern ObjectPress *Press;

// Standard Entity Events
void Press_Update(void);
void Press_LateUpdate(void);
void Press_StaticUpdate(void);
void Press_Draw(void);
void Press_Create(void *data);
void Press_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Press_EditorDraw(void);
void Press_EditorLoad(void);
#endif
void Press_Serialize(void);

// Extra Entity Functions
#if MANIA_USE_PLUS
bool32 Press_CheckCanSuper(bool32 isHUD);
#endif
void Press_Move(void);
void Press_State_Crush(void);
void Press_State_FinalCrush(void);
void Press_HandleMovement(void);
void Press_State_HandleCrates(void);
void Press_DrawHandle(void);

#endif //! OBJ_PRESS_H
