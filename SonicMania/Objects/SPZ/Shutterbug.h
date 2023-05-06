#ifndef OBJ_SHUTTERBUG_H
#define OBJ_SHUTTERBUG_H

#include "Game.h"

// Object Class
struct ObjectShutterbug {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitboxAchievement;
    uint32 pictureCount;
    uint16 aniFrames;
    uint16 snapSfx;
};

// Entity Class
struct EntityShutterbug {
    RSDK_ENTITY
    StateMachine(state);
    uint8 snaps;
    bool32 passThrough;
    int32 snapTimer;
    uint32 numSnaps;
    EntityPlayer *focusTarget;
    Vector2 startPos;
    Vector2 range;
    Vector2 targetPos;
    uint8 turnTimer;
    uint8 moveDir;
    uint8 flickerTimer;
    Hitbox hitboxRange;
    Animator animator;
    Animator overlayAnimator;
};

// Object Struct
extern ObjectShutterbug *Shutterbug;

// Standard Entity Events
void Shutterbug_Update(void);
void Shutterbug_LateUpdate(void);
void Shutterbug_StaticUpdate(void);
void Shutterbug_Draw(void);
void Shutterbug_Create(void *data);
void Shutterbug_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Shutterbug_EditorDraw(void);
void Shutterbug_EditorLoad(void);
#endif
void Shutterbug_Serialize(void);

void Shutterbug_DebugSpawn(void);
void Shutterbug_DebugDraw(void);

void Shutterbug_CheckOffScreen(void);

void Shutterbug_State_Init(void);
void Shutterbug_State_FlyAround(void);
void Shutterbug_State_ShakeFly(void);
void Shutterbug_State_FlyAway(void);

int32 Shutterbug_BounceX(void);
int32 Shutterbug_BounceY(void);

void Shutterbug_CheckFocus(void);
void Shutterbug_TryTakePicture(void);
void Shutterbug_HandleBodyAnim(void);

// Extra Entity Functions

#endif //! OBJ_SHUTTERBUG_H
