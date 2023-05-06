#ifndef OBJ_FUNNEL_H
#define OBJ_FUNNEL_H

#include "Game.h"

// Object Class
struct ObjectFunnel {
    RSDK_OBJECT
    Hitbox hitboxFunnel;
    uint16 unused;
    uint16 sfxFunnel;
    uint16 sfxPimPom;
    uint16 aniFrames;
};

// Entity Class
struct EntityFunnel {
    RSDK_ENTITY
    StateMachine(state);
    uint8 activePlayers;
    int32 playerAngle[PLAYER_COUNT];
    int32 playerXVel[PLAYER_COUNT];
    int32 playerScoreTimer[PLAYER_COUNT];
    int32 playerYVel[PLAYER_COUNT];
    int32 playerTimers[PLAYER_COUNT];
    Animator animator;
};

// Object Struct
extern ObjectFunnel *Funnel;

// Standard Entity Events
void Funnel_Update(void);
void Funnel_LateUpdate(void);
void Funnel_StaticUpdate(void);
void Funnel_Draw(void);
void Funnel_Create(void *data);
void Funnel_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Funnel_EditorDraw(void);
void Funnel_EditorLoad(void);
#endif
void Funnel_Serialize(void);

// Extra Entity Functions
void Funnel_State_None(void);

#endif //! OBJ_FUNNEL_H
