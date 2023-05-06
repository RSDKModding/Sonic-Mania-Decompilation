#ifndef OBJ_CRANE_H
#define OBJ_CRANE_H

#include "Game.h"

typedef enum {
    CRANE_START_UP,
    CRANE_START_DOWN,
} CraneStartTypes;

typedef enum {
    CRANE_PLAYER1_ONLY,
    CRANE_PLAYER2_ONLY,
    CRANE_PLAYER_ALL,
} CranePlayerTypes;

// Object Class
struct ObjectCrane {
    RSDK_OBJECT
    Hitbox hitboxUnused;
    Hitbox hitboxGrab;
    Hitbox hitboxRange;
    uint16 aniFrames;
    uint16 sfxGrab;
};

// Entity Class
struct EntityCrane {
    RSDK_ENTITY
    StateMachine(state);
    Animator frontAnimator;
    Animator backAnimator;
    Animator chainAnimator;
    EntityPlayer *grabbedPlayer;
    uint16 distance;
    uint8 startType;
    uint8 playerType;
    Vector2 startPos;
    uint8 startDir;
    int32 delay;
    int32 timer;
};

// Object Struct
extern ObjectCrane *Crane;

// Standard Entity Events
void Crane_Update(void);
void Crane_LateUpdate(void);
void Crane_StaticUpdate(void);
void Crane_Draw(void);
void Crane_Create(void *data);
void Crane_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Crane_EditorDraw(void);
void Crane_EditorLoad(void);
#endif
void Crane_Serialize(void);

// Extra Entity Functions
void Crane_DebugSpawn(void);
void Crane_DebugDraw(void);

void Crane_HandlePlayerInteractions(void);
void Crane_CheckOffScreen(void);

void Crane_State_CheckOffScreen(void);
void Crane_State_Init(void);
void Crane_State_CheckForPlayers(void);
void Crane_State_LowerToGrab(void);
void Crane_State_CheckGrab(void);
void Crane_State_RiseUp(void);
void Crane_State_ToDest1stHalf(void);
void Crane_State_ToDest2ndHalf(void);
void Crane_State_DropDelay(void);
void Crane_State_ToStart1stHalf(void);
void Crane_State_ToStart2ndHalf(void);

#endif //! OBJ_CRANE_H
