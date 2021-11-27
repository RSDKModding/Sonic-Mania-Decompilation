#ifndef OBJ_PHANTOMEGG_H
#define OBJ_PHANTOMEGG_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(int32 debrisInfo[29], { 4,        0, 0, -0x20000, -0x28000, 1, 0, -0x20000, -0x28000, 2, 0, 0x20000, -0x20000, 3, 0, 0x28000,
                                  -0x20000, 4, 0, -0x10000, -0x10000, 5, 0, 0x10000,  -0x10000, 6, 0, -0x8000, -0x10000 });
    TABLE(int32 attackStateTable1[32], { 0, 0, 0, 0, 1, 0, 0, 2, 0, 1, 3, 0, 2, 0, 0, 4, 0, 0, 1, 0, 2, 0, 0, 3, 0, 0, 1, 0, 0, 2, 0, 4 });
    TABLE(int32 attackStateTable2[32], { 0, 0, 2, 0, 1, 3, 0, 2, 0, 0, 3, 0, 2, 0, 0, 3, 0, 0, 0, 1, 2, 0, 0, 3, 0, 0, 1, 0, 0, 2, 0, 4 });
    TABLE(int32 attackStateTable3[32], { 0, 0, 2, 0, 1, 3, 3, 2, 0, 3, 3, 0, 2, 0, 0, 3, 0, 0, 0, 1, 2, 0, 0, 3, 0, 0, 1, 0, 0, 2, 0, 4 });
    int32 boundsL;
    int32 boundsR;
    int32 boundsM;
    int32 boundsT;
    int32 boundsB;
    int32 boundsStoreL1;
    int32 boundsStoreR1;
    int32 boundsStoreT1;
    int32 boundsStoreB1;
    int32 startScanline;
    int32 endScanline;
    bool32 setupPalette;
    bool32 savedGameProgress;
    uint16 sfxHit;
    uint16 sfxExplosion2;
    uint16 sfxRocketJet;
    uint16 sfxExplosion3;
    uint16 sfxJump;
    uint16 sfxLand;
    uint16 sfxRepel;
    uint16 sfxShield;
    uint16 sfxShock;
    uint16 sfxSummon;
    uint16 sfxMissile;
    uint16 aniFrames;
    bool32 superFlag;
} ObjectPhantomEgg;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int32 type;
    int32 timer;
    int32 invincibilityTimer;
    int32 health;
    int32 field_70;
    int32 attackTimer;
    int32 phantomID;
    int32 palBlendPercent;
    Vector2 field_80;
    int32 field_88;
    int32 field_8C;
    int32 *attackStateTable;
    int32 field_94;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    Animator animator5;
    Animator animator6;
    Animator animator7;
    Hitbox hitbox;
} EntityPhantomEgg;

// Object Struct
extern ObjectPhantomEgg *PhantomEgg;

// Standard Entity Events
void PhantomEgg_Update(void);
void PhantomEgg_LateUpdate(void);
void PhantomEgg_StaticUpdate(void);
void PhantomEgg_Draw(void);
void PhantomEgg_Create(void* data);
void PhantomEgg_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void PhantomEgg_EditorDraw(void);
void PhantomEgg_EditorLoad(void);
#endif
void PhantomEgg_Serialize(void);

// Extra Entity Functions
void PhantomEgg_HandleAnimations(void);
void PhantomEgg_CheckPlayerCollisions(void);
void PhantomEgg_Hit(void);
void PhantomEgg_Explode(Hitbox *hitbox);
void PhantomEgg_HandleNextAttack(void);
void PhantomEgg_SetupScanlineCB(void);
void PhantomEgg_HandlePhantomWarp(uint8 phantomID);
void PhantomEgg_HandleReturnWarp(void);
void PhantomEgg_ScanlineCB(ScanlineInfo *scanlines);

void PhantomEgg_StateDraw_Normal(void);
void PhantomEgg_StateDraw_Cracked(void);

void PhantomEgg_State_SetupArena(void);
void PhantomEgg_State_Unknown1(void);
void PhantomEgg_State_Unknown2(void);
void PhantomEgg_State_Unknown3(void);
void PhantomEgg_State_Unknown4(void);
void PhantomEgg_State_Unknown5(void);
void PhantomEgg_State_Unknown6(void);
void PhantomEgg_State_Unknown7(void);
void PhantomEgg_State_Unknown8(void);
void PhantomEgg_State_Unknown9(void);
void PhantomEgg_State_Unknown10(void);
void PhantomEgg_State_Unknown11(void);
void PhantomEgg_State_Unknown12(void);
void PhantomEgg_State_Unknown13(void);
void PhantomEgg_State_Unknown14(void);
void PhantomEgg_State_Unknown15(void);
void PhantomEgg_State_Destroyed(void);
void PhantomEgg_State_Unknown17(void);
void PhantomEgg_State_Unknown18(void);
void PhantomEgg_State_Unknown19(void);
void PhantomEgg_State_Unknown20(void);
void PhantomEgg_State_Unknown21(void);

void PhantomEgg_SaveGameCB(int32 status);

#endif //!OBJ_PHANTOMEGG_H
