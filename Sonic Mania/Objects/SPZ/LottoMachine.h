#ifndef OBJ_LOTTOMACHINE_H
#define OBJ_LOTTOMACHINE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox1;
    Hitbox hitbox2;
    uint8 activePlayers;
    uint16 sfxPimPom;
    uint16 sfxFail;
    Animator animator;
    uint16 aniFrames;
} ObjectLottoMachine;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 playerCount;
    Entity *playerPtrs[PLAYER_MAX];
    int32 timer;
    int32 field_74;
    int32 field_78;
    int32 playerTimers[PLAYER_MAX];
    int32 field_8C;
    int32 field_90[PLAYER_MAX];
    int32 field_A0;
    int32 field_A4;
    int32 field_A8;
    uint8 drawOrderHigh;
    int32 playerAngles[4];
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    Animator animator5;
} EntityLottoMachine;

// Object Struct
extern ObjectLottoMachine *LottoMachine;

// Standard Entity Events
void LottoMachine_Update(void);
void LottoMachine_LateUpdate(void);
void LottoMachine_StaticUpdate(void);
void LottoMachine_Draw(void);
void LottoMachine_Create(void* data);
void LottoMachine_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void LottoMachine_EditorDraw(void);
void LottoMachine_EditorLoad(void);
#endif
void LottoMachine_Serialize(void);

// Extra Entity Functions
void LottoMachine_ZoneCB(void);
void LottoMachine_CheckPlayerCollisions(void);
void LottoMachine_CheckPlayerCollisions2(void);
void LottoMachine_Unknown4(void);
void LottoMachine_Unknown5(void);
void LottoMachine_Unknown6(void);
void LottoMachine_GiveRings(void);

void LottoMachine_State_Unknown1(void);
void LottoMachine_State_Unknown2(void);
void LottoMachine_State_Unknown3(void);
void LottoMachine_State_Unknown4(void);
void LottoMachine_State_Unknown5(void);
void LottoMachine_State_Unknown6(void);
void LottoMachine_State_Unknown7(void);

#endif //!OBJ_LOTTOMACHINE_H
