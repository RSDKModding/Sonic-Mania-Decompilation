#ifndef OBJ_FUNNEL_H
#define OBJ_FUNNEL_H

#include "SonicMania.h"

// Object Class
struct ObjectFunnel {
    RSDK_OBJECT
    Hitbox hitbox;
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
    int32 playerAngle[PLAYER_MAX];
    int32 playerXVel[PLAYER_MAX];
    int32 playerScoreTimer[PLAYER_MAX];
    int32 playerYVel[PLAYER_MAX];
    int32 playerTimers[PLAYER_MAX];
    Animator animator;
};

// Object Struct
extern ObjectFunnel *Funnel;

// Standard Entity Events
void Funnel_Update(void);
void Funnel_LateUpdate(void);
void Funnel_StaticUpdate(void);
void Funnel_Draw(void);
void Funnel_Create(void* data);
void Funnel_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Funnel_EditorDraw(void);
void Funnel_EditorLoad(void);
#endif
void Funnel_Serialize(void);

// Extra Entity Functions
void Funnel_State_None(void);


#endif //!OBJ_FUNNEL_H
