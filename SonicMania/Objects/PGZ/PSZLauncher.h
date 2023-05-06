#ifndef OBJ_PSZLAUNCHER_H
#define OBJ_PSZLAUNCHER_H

#include "Game.h"

// Object Class
struct ObjectPSZLauncher {
    RSDK_OBJECT
    TABLE(int32 heightTable[33], { 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13, 15, 15, 16, 16, 0 });
    Hitbox hitboxLaunch;
    uint16 aniFrames;
};

// Entity Class
struct EntityPSZLauncher {
    RSDK_ENTITY
    StateMachine(state);
    uint8 stoodPlayers;
    uint8 activePlayers;
    Animator animator;
    uint8 power;
};

// Object Struct
extern ObjectPSZLauncher *PSZLauncher;

// Standard Entity Events
void PSZLauncher_Update(void);
void PSZLauncher_LateUpdate(void);
void PSZLauncher_StaticUpdate(void);
void PSZLauncher_Draw(void);
void PSZLauncher_Create(void *data);
void PSZLauncher_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PSZLauncher_EditorDraw(void);
void PSZLauncher_EditorLoad(void);
#endif
void PSZLauncher_Serialize(void);

// Extra Entity Functions
void PSZLauncher_DebugSpawn(void);
void PSZLauncher_DebugDraw(void);

void PSZLauncher_HandlePlayerCollisions(void);
void PSZLauncher_HandlePlayerInteractions(void);
void PSZLauncher_State_Init(void);
void PSZLauncher_State_Active(void);

#endif //! OBJ_PSZLAUNCHER_H
