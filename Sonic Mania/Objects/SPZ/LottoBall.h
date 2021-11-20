#ifndef OBJ_LOTTOBALL_H
#define OBJ_LOTTOBALL_H

#include "SonicMania.h"

typedef enum {
    LOTTOBALL_BLUE,
    LOTTOBALL_YELLOW,
    LOTTOBALL_MULTI,
    LOTTOBALL_EGGMAN,
    LOTTOBALL_BIG,
    LOTTOBALL_TOTAL,
}LottoBallTypes;

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxLottoBounce;
    uint16 sfxSpew;
} ObjectLottoBall;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    uint8 type;
    uint8 lottoNum;
    int32 ringCount;
    int32 timer;
    Vector2 startPos;
    int32 field_70;
    int32 field_74;
    int32 gravityStrength;
    int32 angleVel;
    Entity *parent;
    int32 screenFlags[4];
    bool32 screenRelative;
    Animator animator1;
    Animator animator2;
    Animator animator3;
} EntityLottoBall;

// Object Struct
extern ObjectLottoBall *LottoBall;

// Standard Entity Events
void LottoBall_Update(void);
void LottoBall_LateUpdate(void);
void LottoBall_StaticUpdate(void);
void LottoBall_Draw(void);
void LottoBall_Create(void* data);
void LottoBall_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void LottoBall_EditorDraw(void);
void LottoBall_EditorLoad(void);
#endif
void LottoBall_Serialize(void);

// Extra Entity Functions
void LottoBall_Unknown1(void);
void LottoBall_Unknown2(void);
void LottoBall_CheckOnScreen(void);
void LottoBall_Unknown4(void);
void LottoBall_Unknown5(void);
void LottoBall_Unknown6(void);
void LottoBall_Unknown7(void);
void LottoBall_Unknown8(void);

#endif //!OBJ_LOTTOBALL_H
