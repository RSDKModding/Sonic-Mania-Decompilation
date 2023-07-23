#ifndef OBJ_TIMEATTACKGATE_H
#define OBJ_TIMEATTACKGATE_H

#include "Game.h"

// Object Class
struct ObjectTimeAttackGate {
    RSDK_OBJECT
    Hitbox hitboxGate;
    uint16 aniFrames;
    EntityTimeAttackGate *restartManager;
    EntityPlayer *triggerPlayer;
    bool32 started;
    int32 isFinished;
    int32 unused1;
    bool32 suppressedTitlecard;
    bool32 disableRecords;
    uint16 sfxSignpost;
    uint16 sfxTeleport;
    int32 teleportChannel;
    StateMachine(startCB);
    StateMachine(endCB);
};

// Entity Class
struct EntityTimeAttackGate {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    bool32 finishLine;
    int32 extendTop;
    int32 extendBottom;
    Vector2 boundsSize;
    Vector2 boundsOffset;
    bool32 topBoundary;
    int32 timer;
    bool32 hasFinished;
    int32 baseRotation;
    int32 spinSpeed;
    int32 spinTimer;
    int32 radius;
    int32 restartTimer;
    int32 fadeTimer;
    Vector2 playerPos;
    Animator baseAnimator;
    Animator topAnimator;
    Animator finAnimator;
};

// Object Struct
extern ObjectTimeAttackGate *TimeAttackGate;

// Standard Entity Events
void TimeAttackGate_Update(void);
void TimeAttackGate_LateUpdate(void);
void TimeAttackGate_StaticUpdate(void);
void TimeAttackGate_Draw(void);
void TimeAttackGate_Create(void *data);
void TimeAttackGate_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void TimeAttackGate_EditorDraw(void);
void TimeAttackGate_EditorLoad(void);
#endif
void TimeAttackGate_Serialize(void);

// Extra Entity Functions
void TimeAttackGate_HandleSpin(void);
void TimeAttackGate_HandleStart(void);
#if MANIA_USE_PLUS
void TimeAttackGate_AddRecord(void);
void TimeAttackGate_WaitSave_Leaderboards(bool32 success);
#endif
void TimeAttackGate_CheckTouch(void);

// States
void TimeAttackGate_State_Gate(void);
void TimeAttackGate_State_Restarter(void);
void TimeAttackGate_State_Fadeout(void);

// Draw States
void TimeAttackGate_Draw_Gate(void);
void TimeAttackGate_Draw_Restarter(void);

#endif //! OBJ_TIMEATTACKGATE_H
