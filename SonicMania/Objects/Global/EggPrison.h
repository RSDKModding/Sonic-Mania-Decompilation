#ifndef OBJ_EGGPRISON_H
#define OBJ_EGGPRISON_H

#include "Game.h"

typedef enum {
    EGGPRISON_NORMAL,
    EGGPRISON_FLYING,
    EGGPRISON_DUD,
    EGGPRISON_RINGS,
    EGGPRISON_TRAP,
    EGGPRISON_SPRING,
} EggPrisonTypes;

// Object Class
struct ObjectEggPrison {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxDestroy;
    uint16 sfxSpring;
};

// Entity Class
struct EntityEggPrison {
    RSDK_ENTITY
    StateMachine(state);
    EggPrisonTypes type;
    int32 timer;
    int32 originY;
    int32 buttonPos;
    int32 notSolid;
    int32 checkTileCollisions;
    Hitbox hitboxSolid;
    Hitbox hitboxButton;
    Hitbox hitboxButtonTrigger;
    Animator capsuleAnimator;
    Animator propellerAnimator;
    Animator panelAnimator;
    Animator buttonAnimator;
};

// Object Struct
extern ObjectEggPrison *EggPrison;

// Standard Entity Events
void EggPrison_Update(void);
void EggPrison_LateUpdate(void);
void EggPrison_StaticUpdate(void);
void EggPrison_Draw(void);
void EggPrison_Create(void *data);
void EggPrison_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void EggPrison_EditorDraw(void);
void EggPrison_EditorLoad(void);
#endif
void EggPrison_Serialize(void);

// Extra Entity Functions
void EggPrison_HandleMovement(void);

void EggPrison_State_Init(void);
void EggPrison_State_Idle(void);
void EggPrison_State_Opened(void);
void EggPrison_State_Explode(void);
void EggPrison_State_SetupActClear(void);
void EggPrison_State_FlyOffScreen(void);

#endif //! OBJ_EGGPRISON_H
