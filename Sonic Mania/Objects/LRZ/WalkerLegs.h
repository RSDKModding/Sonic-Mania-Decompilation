#ifndef OBJ_WALKERLEGS_H
#define OBJ_WALKERLEGS_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    uint16 aniFrames;
    uint16 particleFrames;
    uint16 sfxWalkerLegs;
    uint16 sfxWalkerLegs2;
    Animator animator1;
    Animator animator2;
    bool32 hasAchievement;
} ObjectWalkerLegs;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    uint16 steps;
    int32 activePlayers[2];
    Vector2 field_68[3];
    Vector2 field_80[2];
    Vector2 field_90[2];
    int32 field_A0;
    int32 field_A4;
    int32 startAngle;
    int32 field_AC;
    bool32 flag;
    int32 field_B4;
    uint16 stepCount;
    int32 field_BC[2];
    int32 timer;
} EntityWalkerLegs;

// Object Struct
extern ObjectWalkerLegs *WalkerLegs;

// Standard Entity Events
void WalkerLegs_Update(void);
void WalkerLegs_LateUpdate(void);
void WalkerLegs_StaticUpdate(void);
void WalkerLegs_Draw(void);
void WalkerLegs_Create(void* data);
void WalkerLegs_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void WalkerLegs_EditorDraw(void);
void WalkerLegs_EditorLoad(void);
#endif
void WalkerLegs_Serialize(void);

// Extra Entity Functions
void WalkerLegs_DrawSprites(void);
void WalkerLegs_CheckOnScreen(void);
void WalkerLegs_CheckPlayerCollisions(void);
void WalkerLegs_CheckObjectCrush(void);
void WalkerLegs_CheckPlayerStood(void);
void WalkerLegs_CheckTileCollisions(void);
void WalkerLegs_CheckStoodLava(void);
void WalkerLegs_CreateDebris(bool32 flag1, bool32 flag2);
void WalkerLegs_CreateSmoke(bool32 flag);

void WalkerLegs_State_Unknown1(void);
void WalkerLegs_State_Unknown2(void);
void WalkerLegs_State_Unknown3(void);
void WalkerLegs_State_Unknown4(void);

#endif //!OBJ_WALKERLEGS_H
