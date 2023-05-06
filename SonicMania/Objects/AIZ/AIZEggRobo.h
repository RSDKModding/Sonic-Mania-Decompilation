#ifndef OBJ_AIZEGGROBO_H
#define OBJ_AIZEGGROBO_H

#include "Game.h"

// Object Class
struct ObjectAIZEggRobo {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityAIZEggRobo {
    RSDK_ENTITY
    StateMachine(state); // unused
    bool32 oscillate;
    bool32 forKnux;
    int32 unused1;
    Vector2 movePos;
    Vector2 startPos;
    int32 unused2;
    int32 oscillateOffset;
    int32 unused3;
    int32 unused4;
    Animator animatorBody;
    Animator animatorArm;
    Animator animatorLegs;
};

// Object Struct
extern ObjectAIZEggRobo *AIZEggRobo;

// Standard Entity Events
void AIZEggRobo_Update(void);
void AIZEggRobo_LateUpdate(void);
void AIZEggRobo_StaticUpdate(void);
void AIZEggRobo_Draw(void);
void AIZEggRobo_Create(void *data);
void AIZEggRobo_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void AIZEggRobo_EditorDraw(void);
void AIZEggRobo_EditorLoad(void);
#endif
void AIZEggRobo_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_AIZEGGROBO_H
