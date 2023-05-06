#ifndef OBJ_PULLSWITCH_H
#define OBJ_PULLSWITCH_H

#include "Game.h"

// Object Class
struct ObjectPullSwitch {
    RSDK_OBJECT
    Hitbox hitbox;
    int32 unused;
    uint16 aniFrames;
    uint16 sfxButton;
    uint16 sfxSmogClear;
};

// Entity Class
struct EntityPullSwitch {
    RSDK_ENTITY
    StateMachine(state);
    int32 pullPos;
    int32 sprY;
    int32 sprHeight;
    bool32 activated;
    uint8 playerTimers[PLAYER_COUNT];
    uint8 activePlayers;
    Vector2 handlePos;
    Animator handleAnimator;
    Animator dispenserAnimator;
    Animator chainAnimator;
};

// Object Struct
extern ObjectPullSwitch *PullSwitch;

// Standard Entity Events
void PullSwitch_Update(void);
void PullSwitch_LateUpdate(void);
void PullSwitch_StaticUpdate(void);
void PullSwitch_Draw(void);
void PullSwitch_Create(void *data);
void PullSwitch_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PullSwitch_EditorDraw(void);
void PullSwitch_EditorLoad(void);
#endif
void PullSwitch_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_PULLSWITCH_H
