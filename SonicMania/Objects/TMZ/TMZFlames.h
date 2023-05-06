#ifndef OBJ_TMZFLAMES_H
#define OBJ_TMZFLAMES_H

#include "Game.h"

// Object Class
struct ObjectTMZFlames {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityTMZFlames {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 offset;
    Animator animator;
};

// Object Struct
extern ObjectTMZFlames *TMZFlames;

// Standard Entity Events
void TMZFlames_Update(void);
void TMZFlames_LateUpdate(void);
void TMZFlames_StaticUpdate(void);
void TMZFlames_Draw(void);
void TMZFlames_Create(void *data);
void TMZFlames_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void TMZFlames_EditorDraw(void);
void TMZFlames_EditorLoad(void);
#endif
void TMZFlames_Serialize(void);

// Extra Entity Functions
void TMZFlames_State_Delay(void);
void TMZFlames_State_EnterFlames(void);
void TMZFlames_State_FlamesEnlarge(void);

#endif //! OBJ_TMZFLAMES_H
