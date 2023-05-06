#ifndef OBJ_POPCORNMACHINE_H
#define OBJ_POPCORNMACHINE_H

#include "Game.h"

typedef enum {
    POPCORNMACHINE_LEFT,
    POPCORNMACHINE_RIGHT,
    POPCORNMACHINE_BOTH,
} PopcornMachineTypes;

// Object Class
struct ObjectPopcornMachine {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitboxEntryL;
    Hitbox hitboxEntryR;
    int32 unused1;
    int32 unused2;
    uint16 sfxFanStart;
    uint16 sfxShoot;
    uint16 sfxPopcornLaunch;
};

// Entity Class
struct EntityPopcornMachine {
    RSDK_ENTITY
    StateMachine(state);
    uint8 type;
    uint8 height;
    int32 timer;
    EntityPopcornMachine *parent;
    EntityPlayer *player;
    int32 bounds[PLAYER_COUNT];
    Vector2 dispenserHolderPos;
    Vector2 dispenserPos;
    Animator animator;
    Hitbox hitboxSideL;
    Hitbox hitboxSideR;
    Hitbox hitboxTop;
};

// Object Struct
extern ObjectPopcornMachine *PopcornMachine;

// Standard Entity Events
void PopcornMachine_Update(void);
void PopcornMachine_LateUpdate(void);
void PopcornMachine_StaticUpdate(void);
void PopcornMachine_Draw(void);
void PopcornMachine_Create(void *data);
void PopcornMachine_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PopcornMachine_EditorDraw(void);
void PopcornMachine_EditorLoad(void);
#endif
void PopcornMachine_Serialize(void);

// Extra Entity Functions
void PopcornMachine_LinkPlayer(EntityPlayer *player);
void PopcornMachine_CheckPlayerCollisions(void);

void PopcornMachine_State_Idle(void);
void PopcornMachine_State_ShowDispenser(void);
void PopcornMachine_State_Shaking(void);
void PopcornMachine_State_HideDispenser(void);

void PopcornMachine_StateController_ReadyPlayer(void);
void PopcornMachine_StateController_RisePlayer(void);
void PopcornMachine_StateController_FirePlayer(void);
void PopcornMachine_StateController_HandleFinish(void);

#endif //! OBJ_POPCORNMACHINE_H
