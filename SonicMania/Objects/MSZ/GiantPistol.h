#ifndef OBJ_GIANTPISTOL_H
#define OBJ_GIANTPISTOL_H

#include "Game.h"

// Object Class
struct ObjectGiantPistol {
    RSDK_OBJECT
#if MANIA_USE_PLUS
    bool32 inCutscene;
#endif
    Hitbox hitboxFront;
    Hitbox hitboxEntry;
    Hitbox hitboxRear;
    uint16 sfxClack;
    uint16 sfxCannonFire;
    uint16 aniFrames;
};

// Entity Class
struct EntityGiantPistol {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 activePlayers;
    int32 playerPos;
    int32 rotationVel;
    Vector2 startPos;
    Vector2 pivot;
    Animator pistolAnimator;
    Animator pistolEntryAnimator;
    Animator chamberAnimator;
    Animator hammerAnimator;
    Animator handLowAnimator;
    Animator handHighAnimator;
    Animator triggerAnimator;
    Animator triggerFingerAnimator;
};

// Object Struct
extern ObjectGiantPistol *GiantPistol;

// Standard Entity Events
void GiantPistol_Update(void);
void GiantPistol_LateUpdate(void);
void GiantPistol_StaticUpdate(void);
void GiantPistol_Draw(void);
void GiantPistol_Create(void *data);
void GiantPistol_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void GiantPistol_EditorDraw(void);
void GiantPistol_EditorLoad(void);
#endif
void GiantPistol_Serialize(void);

// Extra Entity Functions
void GiantPistol_State_AwaitPlayerEntry(void);
void GiantPistol_State_CloseChamber(void);
void GiantPistol_State_SpinGun(void);
void GiantPistol_State_Aiming(void);
void GiantPistol_State_FiredPlayers(void);

#if MANIA_USE_PLUS
void GiantPistol_PlayerState_PistolAir(void);
void GiantPistol_PlayerState_PistolGround(void);
#endif

#endif //! OBJ_GIANTPISTOL_H
