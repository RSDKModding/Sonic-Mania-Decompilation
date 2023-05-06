#ifndef OBJ_FAN_H
#define OBJ_FAN_H

#include "Game.h"

typedef enum {
    FAN_V,
    FAN_H,
} FanTypes;

typedef enum {
    FAN_ACTIVATE_NONE,
    FAN_ACTIVATE_INTERVAL,
    FAN_ACTIVATE_PLATFORM,
    FAN_ACTIVATE_BUTTON,
} FanActivationTypes;

typedef enum {
    FAN_DEACTIVATE_NONE,
    FAN_DEACTIVATE_BUTTON,
} FanDeactivationTypes;

// Object Class
struct ObjectFan {
    RSDK_OBJECT
    int32 activePlayers;
    uint8 unused;
    int32 minVelocity;
    Hitbox hitboxTop;
    Hitbox hitboxBottom;
    Hitbox hitboxSides;
    Hitbox playerHitbox;
    uint16 aniFrames;
    uint16 sfxFan;
    bool32 playingFanSfx;
};

// Entity Class
struct EntityFan {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateActivate);
    StateMachine(stateDeactivate);
    uint8 type;
    uint8 activation;
    uint8 deactivation;
    int32 size;
    uint16 interval;
    uint16 intervalOffset;
    uint16 duration;
    uint16 delay;
    int32 buttonTag;
    EntityButton *taggedButton;
    Animator animator;
};

// Object Struct
extern ObjectFan *Fan;

// Standard Entity Events
void Fan_Update(void);
void Fan_LateUpdate(void);
void Fan_StaticUpdate(void);
void Fan_Draw(void);
void Fan_Create(void *data);
void Fan_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Fan_EditorDraw(void);
void Fan_EditorLoad(void);
#endif
void Fan_Serialize(void);

// Extra Entity Functions
void Fan_SetupTagLink(void);

void Fan_HandlePlayerInteractions_Top(void);
void Fan_HandlePlayerInteractions_Bottom(void);
void Fan_HandlePlayerInteractions_Left(void);
void Fan_HandlePlayerInteractions_Right(void);

void Fan_State_Started(void);
void Fan_State_Stopped(void);

void Fan_Activate_Interval(void);
void Fan_Activate_Button(void);
void Fan_Deactivate_Button(void);
void Fan_Activate_Platform(void);
void Fan_Activate(void);

#endif //! OBJ_FAN_H
