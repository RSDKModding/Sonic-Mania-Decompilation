#ifndef OBJ_BSS_PLAYER_H
#define OBJ_BSS_PLAYER_H

#include "Game.h"

// Object Class
struct ObjectBSS_Player {
    RSDK_OBJECT
    uint16 jumpPressState;
    uint16 unused1; // set to 4 when hitting a yellow sphere, never *used* though
    uint16 sonicFrames;
    uint16 tailsFrames;
    uint16 knuxFrames;
    uint16 mightyFrames;
    uint16 rayFrames;
    uint16 sfxJump;
};

// Entity Class
struct EntityBSS_Player {
    RSDK_ENTITY
    int32 gravityStrength;
    bool32 sidekick;
    StateMachine(stateInput);
    int32 controllerID;
    bool32 up;
    bool32 down;
    bool32 left;
    bool32 right;
    bool32 jumpPress;
    Animator animator;
    Animator tailAnimator;
    uint16 aniFrames;
};

// Object Struct
extern ObjectBSS_Player *BSS_Player;

// Standard Entity Events
void BSS_Player_Update(void);
void BSS_Player_LateUpdate(void);
void BSS_Player_StaticUpdate(void);
void BSS_Player_Draw(void);
void BSS_Player_Create(void *data);
void BSS_Player_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void BSS_Player_EditorDraw(void);
void BSS_Player_EditorLoad(void);
#endif
void BSS_Player_Serialize(void);

// Extra Entity Functions
void BSS_Player_Input_P1(void);
void BSS_Player_Input_P2(void);

#endif //! OBJ_BSS_PLAYER_H
