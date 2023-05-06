#ifndef OBJ_LOTTOMACHINE_H
#define OBJ_LOTTOMACHINE_H

#include "Game.h"

// Object Class
struct ObjectLottoMachine {
    RSDK_OBJECT
    Hitbox hitboxBottom;
    Hitbox hitboxMotor;
    uint8 activePlayers;
    uint16 sfxPimPom;
    uint16 sfxFail;
    Animator shineAnimator;
    uint16 aniFrames;
};

// Entity Class
struct EntityLottoMachine {
    RSDK_ENTITY
    StateMachine(state);
    int32 playerCount;
    EntityPlayer *playerPtrs[PLAYER_COUNT];
    int32 timer;
    int32 spinSpeed;
    int32 playerDir;
    int32 playerTimers[PLAYER_COUNT];
    int32 collectedBallCount;
    int32 collectedBallTypes[4];
    int32 chutePos;
    int32 chuteTargetPos;
    int32 chuteVel;
    uint8 drawGroupHigh;
    int32 playerAngles[PLAYER_COUNT];
    Animator machineAnimator;
    Animator supportAnimator;
    Animator chuteAnimator;
    Animator motorAnimator;
    Animator glassAnimator;
};

// Object Struct
extern ObjectLottoMachine *LottoMachine;

// Standard Entity Events
void LottoMachine_Update(void);
void LottoMachine_LateUpdate(void);
void LottoMachine_StaticUpdate(void);
void LottoMachine_Draw(void);
void LottoMachine_Create(void *data);
void LottoMachine_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void LottoMachine_EditorDraw(void);
void LottoMachine_EditorLoad(void);
#endif
void LottoMachine_Serialize(void);

// Extra Entity Functions
void LottoMachine_VSSwap_CheckBusy(void);
void LottoMachine_CheckPlayerCollisions(void);
void LottoMachine_CheckPlayerCollisions_Bottom(void);
void LottoMachine_HandleMotor(void);
void LottoMachine_SetupBalls(void);
void LottoMachine_SetupUIBalls(void);
void LottoMachine_GiveRings(void);

void LottoMachine_State_Startup(void);
void LottoMachine_State_HandleBallCollect(void);
void LottoMachine_State_CollectBall(void);
void LottoMachine_State_HandleBallCollected(void);
void LottoMachine_State_DropPlayers(void);
void LottoMachine_State_ReleasePlayers(void);
void LottoMachine_State_StopSpinning(void);

#endif //! OBJ_LOTTOMACHINE_H
