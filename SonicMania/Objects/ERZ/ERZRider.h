#ifndef OBJ_ERZRIDER_H
#define OBJ_ERZRIDER_H

#include "Game.h"

// Object Class
struct ObjectERZRider {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityERZRider {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    Vector2 startPos;
    bool32 isJimmy;
    int32 unused1;
    int32 unused2;
    Animator mainAnimator;
    Animator jimmyAnimator;
    Animator wheelAnimator;
    Animator thrustAnimator;
    Animator unusedAnimator1;
    Animator unusedAnimator2;
};

// Object Struct
extern ObjectERZRider *ERZRider;

// Standard Entity Events
void ERZRider_Update(void);
void ERZRider_LateUpdate(void);
void ERZRider_StaticUpdate(void);
void ERZRider_Draw(void);
void ERZRider_Create(void *data);
void ERZRider_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void ERZRider_EditorDraw(void);
void ERZRider_EditorLoad(void);
#endif
void ERZRider_Serialize(void);

// Extra Entity Functions
void ERZRider_Draw_Rider(void);

#endif //! OBJ_ERZRIDER_H
