#ifndef OBJ_TIMEATTACKGATE_H
#define OBJ_TIMEATTACKGATE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    ushort aniFrames;
    Entity *activeEntity;
    EntityPlayer *playerPtr;
    bool32 started;
    int field_1C;
    int field_20;
    bool32 suppressedTitlecard;
    bool32 debugEnabled;
    ushort sfxSignpost;
    ushort sfxTeleport;
    int dword30;
    StateMachine(startCB);
    StateMachine(endCB);
} ObjectTimeAttackGate;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    bool32 finishLine;
    int extendTop;
    int extendBottom;
    Vector2 boundsSize;
    Vector2 boundsOffset;
    int topBoundary;
    int timer;
    int field_84;
    int field_88;
    int spinSpeed;
    int spinTimer;
    int radius;
    int restartTimer;
    int fadeTimer;
    Vector2 playerPos;
    Animator baseAnimator;
    Animator topAnimator;
    Animator finAnimator;
} EntityTimeAttackGate;

// Object Struct
extern ObjectTimeAttackGate *TimeAttackGate;

// Standard Entity Events
void TimeAttackGate_Update(void);
void TimeAttackGate_LateUpdate(void);
void TimeAttackGate_StaticUpdate(void);
void TimeAttackGate_Draw(void);
void TimeAttackGate_Create(void* data);
void TimeAttackGate_StageLoad(void);
void TimeAttackGate_EditorDraw(void);
void TimeAttackGate_EditorLoad(void);
void TimeAttackGate_Serialize(void);

// Extra Entity Functions
void TimeAttackGate_HandleSpin(void);
void TimeAttackGate_HandleStart(void);
void TimeAttackGate_Unknown1(void);
void TimeAttackGate_LeaderboardCB(int status);
void TimeAttackGate_CheckTouch(void);

void TimeAttackGate_State_Main(void);
void TimeAttackGate_State_Restarter(void);
void TimeAttackGate_Unknown4(void);

void TimeAttackGate_StateDraw_Main(void);
void TimeAttackGate_StateDraw_Restarter(void);

#endif //!OBJ_TIMEATTACKGATE_H
