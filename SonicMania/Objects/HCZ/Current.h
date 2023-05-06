#ifndef OBJ_CURRENT_H
#define OBJ_CURRENT_H

#include "Game.h"

typedef enum {
    CURRENT_C_LEFT,
    CURRENT_C_RIGHT,
    CURRENT_C_UP,
    CURRENT_C_DOWN,
    CURRENT_W_LEFT,
    CURRENT_W_RIGHT,
    CURRENT_W_UP,
    CURRENT_W_DOWN,
} CurrentTypes;

typedef enum {
    CURRENT_CHILD_NONE,
    CURRENT_CHILD_WIND,
    CURRENT_CHILD_BUBBLE,
} CurrentChildTypes;

// Object Class
struct ObjectCurrent {
    RSDK_OBJECT
    uint8 activePlayers;
    Animator animator;
    uint16 aniFrames;
    uint16 sfxRush;
};

// Entity Class
struct EntityCurrent {
    RSDK_ENTITY
    StateMachine(state);
    CurrentTypes type;
    Vector2 size;
    uint8 strength;
    Hitbox hitbox;
    uint8 activePlayers;
    Vector2 playerPositions[PLAYER_COUNT];
    int32 buttonTag;
    EntityButton *taggedButton;
    bool32 activated;
    PlaneFilterTypes planeFilter;
    bool32 waterOnly;
    bool32 fbzAchievement;
    Animator animator;
};

// Object Struct
extern ObjectCurrent *Current;

// Standard Entity Events
void Current_Update(void);
void Current_LateUpdate(void);
void Current_StaticUpdate(void);
void Current_Draw(void);
void Current_Create(void *data);
void Current_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Current_EditorDraw(void);
void Current_EditorLoad(void);
#endif
void Current_Serialize(void);

// Extra Entity Functions
void Current_SetupTagLink(void);
Vector2 Current_GetBubbleSpawnPosHorizontal(uint8 right);
Vector2 Current_GetBubbleSpawnPosVertical(uint8 down);

void Current_State_WaterLeft(void);
void Current_State_WaterRight(void);
void Current_State_WaterUp(void);
void Current_State_WaterDown(void);
void Current_State_PushLeft(void);
void Current_State_PushRight(void);
void Current_State_PushUp(void);
void Current_State_PushDown(void);
void Current_State_Child(void);

void Current_PlayerState_Left(void);
void Current_PlayerState_Right(void);
void Current_PlayerState_Up(void);
void Current_PlayerState_Down(void);

#endif //! OBJ_CURRENT_H
