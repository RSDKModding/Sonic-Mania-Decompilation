#ifndef OBJ_CPZBOSS_H
#define OBJ_CPZBOSS_H

#include "Game.h"

typedef enum {
    CPZBOSS_PLAYER,
    CPZBOSS_EGGMAN,
} CPZBossTypes;

// Object Class
struct ObjectCPZBoss {
    RSDK_OBJECT
    Hitbox hitboxPlayerTrigger;
    EntityPuyoMatch *managers[2];
    uint16 hudSlotID;
    uint16 aniFrames;
    uint16 playerFrames;
    uint16 sfxExplosion;
};

// Entity Class
struct EntityCPZBoss {
    RSDK_ENTITY
    StateMachine(state);
    uint8 type;
    int32 timer;
    Vector2 startPos;
    Vector2 explosionPos;
    Animator panelAnimator;
    Animator enterAnimator;
    Animator characterAnimator;
};

// Object Struct
extern ObjectCPZBoss *CPZBoss;

// Standard Entity Events
void CPZBoss_Update(void);
void CPZBoss_LateUpdate(void);
void CPZBoss_StaticUpdate(void);
void CPZBoss_Draw(void);
void CPZBoss_Create(void *data);
void CPZBoss_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void CPZBoss_EditorDraw(void);
void CPZBoss_EditorLoad(void);
#endif
void CPZBoss_Serialize(void);

// Extra Entity Functions
void CPZBoss_DrawHook_SetupPuyoHUD(void);
void CPZBoss_DrawHook_RemovePuyoHUD(void);

void CPZBoss_Explode_Eggman(void);
void CPZBoss_Explode_Player(void);

bool32 CPZBoss_CheckMatchReset(void);

void CPZBoss_State_SetupArena(void);
void CPZBoss_State_EnterPlayer(void);
void CPZBoss_State_CheckPlayerReady(void);
void CPZBoss_State_PlayPlayerEnterAnim(void);
void CPZBoss_State_SetupMatch(void);
void CPZBoss_State_HandleMatch_Player(void);
void CPZBoss_State_HandleMatch_Eggman(void);
void CPZBoss_State_HandleMatchFinish_EggmanLose(void);
void CPZBoss_State_PlayerWin(void);
void CPZBoss_State_HandleMatchFinish_PlayerLose(void);
void CPZBoss_State_EggmanFall(void);
void CPZBoss_State_PlayerExit(void);
void CPZBoss_State_Destroyed(void);

#endif //! OBJ_CPZBOSS_H
