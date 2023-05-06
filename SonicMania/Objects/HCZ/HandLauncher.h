#ifndef OBJ_HANDLAUNCHER_H
#define OBJ_HANDLAUNCHER_H

#include "Game.h"

// Object Class
struct ObjectHandLauncher {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitboxRange;
    Hitbox hitboxGrab;
    Hitbox hitboxUnused;
    bool32 dunkeyMode;
    uint16 sfxDunkey;
};

// Entity Class
struct EntityHandLauncher {
    RSDK_ENTITY
    StateMachine(state);
    int32 speed;
    uint8 activePlayers;
    Vector2 startPos;
    Vector2 playerPos;
    Vector2 hiddenPos;
    int32 timer;
    int32 grabDelay;
    Animator baseAnimator;
    Animator handAnimator;
};

// Object Struct
extern ObjectHandLauncher *HandLauncher;

// Standard Entity Events
void HandLauncher_Update(void);
void HandLauncher_LateUpdate(void);
void HandLauncher_StaticUpdate(void);
void HandLauncher_Draw(void);
void HandLauncher_Create(void *data);
void HandLauncher_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void HandLauncher_EditorDraw(void);
void HandLauncher_EditorLoad(void);
#endif
void HandLauncher_Serialize(void);

// Extra Entity Functions
void HandLauncher_CheckPlayerCollisions(void);
bool32 HandLauncher_CheckPlayerInRange(void);
void HandLauncher_ReleasePlayers(void);
void HandLauncher_HandleGrabbedPlayers(void);

void HandLauncher_State_Init(void);
void HandLauncher_State_AwaitPlayer(void);
void HandLauncher_State_TryGrabPlayer(void);
void HandLauncher_State_GrabbedPlayer(void);

#endif //! OBJ_HANDLAUNCHER_H
