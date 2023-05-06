#ifndef OBJ_TVPOLE_H
#define OBJ_TVPOLE_H

#include "Game.h"

// Object Class
struct ObjectTVPole {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityTVPole {
    RSDK_ENTITY
    StateMachine(state);
    uint16 length;
    int32 activePlayers;
    int32 playerTimers[PLAYER_COUNT];
    Hitbox hitbox;
    Animator animator;
};

// Object Struct
extern ObjectTVPole *TVPole;

// Standard Entity Events
void TVPole_Update(void);
void TVPole_LateUpdate(void);
void TVPole_StaticUpdate(void);
void TVPole_Draw(void);
void TVPole_Create(void *data);
void TVPole_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void TVPole_EditorDraw(void);
void TVPole_EditorLoad(void);
#endif
void TVPole_Serialize(void);

// Extra Entity Functions
void TVPole_State_CheckGrab(void);
void TVPole_State_ForceRelease(void);

#endif //! OBJ_TVPOLE_H
