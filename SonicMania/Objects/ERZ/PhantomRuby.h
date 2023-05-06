#ifndef OBJ_PHANTOMRUBY_H
#define OBJ_PHANTOMRUBY_H

#include "Game.h"

typedef enum {
    RUBYSFX_NONE,
    RUBYSFX_ATTACK1,
    RUBYSFX_ATTACK2,
    RUBYSFX_ATTACK3,
    RUBYSFX_ATTACK4,
    RUBYSFX_ATTACK5,
    RUBYSFX_ATTACK6,
    RUBYSFX_REDCUBE,
} RubySfxIDs;

// Object Class
struct ObjectPhantomRuby {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxL[7];
    uint16 sfxR[7];
};

// Entity Class
struct EntityPhantomRuby {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 startPos;
    int32 timer;
    bool32 flashFinished;
    bool32 hasFlashed;
    int32 sfx;
    int32 unused1;
    int32 unused2;
    Animator rubyAnimator;
    Animator flashAnimator;
};

// Object Struct
extern ObjectPhantomRuby *PhantomRuby;

// Standard Entity Events
void PhantomRuby_Update(void);
void PhantomRuby_LateUpdate(void);
void PhantomRuby_StaticUpdate(void);
void PhantomRuby_Draw(void);
void PhantomRuby_Create(void *data);
void PhantomRuby_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PhantomRuby_EditorDraw(void);
void PhantomRuby_EditorLoad(void);
#endif
void PhantomRuby_Serialize(void);

// Extra Entity Functions
void PhantomRuby_PlaySfx(uint8 sfxID);
void PhantomRuby_SetupFlash(EntityPhantomRuby *ruby);
void PhantomRuby_State_FinishedFlash(void);
void PhantomRuby_State_PlaySfx(void);
void PhantomRuby_State_Oscillate(void);
void PhantomRuby_State_MoveGravity(void);
void PhantomRuby_State_MoveRotateGravity(void);
void PhantomRuby_State_MoveRotateGravity_CheckGround(void);
#if MANIA_USE_PLUS
void PhantomRuby_State_MoveToPos(void);
#endif
void PhantomRuby_State_RotateToOrigin(void);

#endif //! OBJ_PHANTOMRUBY_H
