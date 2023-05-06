#ifndef OBJ_GRABBER_H
#define OBJ_GRABBER_H

#include "Game.h"

// Object Class
struct ObjectGrabber {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitboxGrab;
    Hitbox hitboxRange;
    uint16 aniFrames;
    uint16 sfxGrab;
    uint16 sfxDestroy;
};

// Entity Class
struct EntityGrabber {
    RSDK_ENTITY
    StateMachine(state);
    Animator bodyAnimator;
    Animator clampAnimator;
    Animator wheelAnimator;
    EntityPlayer *grabbedPlayer;
    uint16 struggleDelay;
    uint16 struggleTimer;
    uint16 struggleFlags;
    uint16 grabDelay;
    Vector2 startPos;
    uint8 startDir;
    int32 turnTimer;
    int32 timer;
};

// Object Struct
extern ObjectGrabber *Grabber;

// Standard Entity Events
void Grabber_Update(void);
void Grabber_LateUpdate(void);
void Grabber_StaticUpdate(void);
void Grabber_Draw(void);
void Grabber_Create(void *data);
void Grabber_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Grabber_EditorDraw(void);
void Grabber_EditorLoad(void);
#endif
void Grabber_Serialize(void);

// Extra Entity Functions
void Grabber_DebugDraw(void);
void Grabber_DebugSpawn(void);

void Grabber_CheckPlayerCollisions(void);
void Grabber_CheckOffScreen(void);
void Grabber_HandleExplode(void);

void Grabber_State_Init(void);
void Grabber_State_CheckForGrab(void);
void Grabber_State_GrabDelay(void);
void Grabber_State_HandleTurn(void);
void Grabber_State_TryToGrab(void);
void Grabber_State_RiseUp(void);
void Grabber_State_GrabbedPlayer(void);
void Grabber_State_Struggle(void);
void Grabber_State_PlayerEscaped(void);

#endif //! OBJ_GRABBER_H
