#ifndef OBJ_LOTTOBALL_H
#define OBJ_LOTTOBALL_H

#include "Game.h"

typedef enum {
    LOTTOBALL_BLUE,
    LOTTOBALL_YELLOW,
    LOTTOBALL_MULTI,
    LOTTOBALL_EGGMAN,
    LOTTOBALL_BIG,
    LOTTOBALL_TOTAL,
} LottoBallTypes;

// Object Class
struct ObjectLottoBall {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxLottoBounce;
    uint16 sfxSpew;
};

// Entity Class
struct EntityLottoBall {
    RSDK_ENTITY
    StateMachine(state);
    uint8 type;
    uint8 lottoNum;
    int32 ringCount;
    int32 timer;
    Vector2 startPos;
    Vector2 bounds;
    int32 gravityStrength;
    int32 angleVel;
    EntityLottoMachine *parent;
    int32 isVisible[PLAYER_COUNT];
    bool32 isUIBall;
    Animator ballAnimator;
    Animator leftNumAnimator;
    Animator rightNumAnimator;
};

// Object Struct
extern ObjectLottoBall *LottoBall;

// Standard Entity Events
void LottoBall_Update(void);
void LottoBall_LateUpdate(void);
void LottoBall_StaticUpdate(void);
void LottoBall_Draw(void);
void LottoBall_Create(void *data);
void LottoBall_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void LottoBall_EditorDraw(void);
void LottoBall_EditorLoad(void);
#endif
void LottoBall_Serialize(void);

// Extra Entity Functions
void LottoBall_CheckOffScreen(void);

void LottoBall_State_FallIntoMachine(void);
void LottoBall_State_InMachine(void);
void LottoBall_State_Collected(void);
void LottoBall_State_CollectFall(void);

void LottoBall_State_SetupUIBall(void);
void LottoBall_State_EnterUIBall(void);
void LottoBall_State_ShowUIBall(void);

#endif //! OBJ_LOTTOBALL_H
