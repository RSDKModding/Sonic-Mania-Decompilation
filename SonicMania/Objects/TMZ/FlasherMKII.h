#ifndef OBJ_FLASHERMKII_H
#define OBJ_FLASHERMKII_H

#include "Game.h"

typedef enum {
    FLASHERMKII_ORIENTATION_UP,
    FLASHERMKII_ORIENTATION_DOWN,
    FLASHERMKII_ORIENTATION_RIGHT,
    FLASHERMKII_ORIENTATION_LEFT,
} FlasherMKIIOrientations;

// Object Class
struct ObjectFlasherMKII {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxFlop;
    uint16 sfxZap;
};

// Entity Class
struct EntityFlasherMKII {
    RSDK_ENTITY
    StateMachine(state);
    uint8 orientation;
    int32 type;
    int32 timer;
    Vector2 startPos;
    Animator animator;
};

// Object Struct
extern ObjectFlasherMKII *FlasherMKII;

// Standard Entity Events
void FlasherMKII_Update(void);
void FlasherMKII_LateUpdate(void);
void FlasherMKII_StaticUpdate(void);
void FlasherMKII_Draw(void);
void FlasherMKII_Create(void *data);
void FlasherMKII_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void FlasherMKII_EditorDraw(void);
void FlasherMKII_EditorLoad(void);
#endif
void FlasherMKII_Serialize(void);

// Extra Entity Functions
void FlasherMKII_DebugSpawn(void);
void FlasherMKII_DebugDraw(void);

void FlasherMKII_CheckOffScreen(void);

void FlasherMKII_HandlePlayerCollisions(void);
void FlasherMKII_HandleHarmPlayerCollisions(void);

void FlasherMKII_State_Idle(void);
void FlasherMKII_State_Moving(void);
void FlasherMKII_State_WeakFlash(void);
void FlasherMKII_State_StrongFlash(void);
void FlasherMKII_State_FinishedFlashing(void);

#endif //! OBJ_FLASHERMKII_H
