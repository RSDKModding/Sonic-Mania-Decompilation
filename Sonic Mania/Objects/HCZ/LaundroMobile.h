#ifndef OBJ_LAUNDROMOBILE_H
#define OBJ_LAUNDROMOBILE_H

#include "SonicMania.h"

typedef enum {
    LAUNDROMOBILE_BOSS,
    LAUNDROMOBILE_BOMB,
    LAUNDROMOBILE_PHASE2,
    LAUNDROMOBILE_LOOPPOINT,
    LAUNDROMOBILE_BLOCK,
    LAUNDROMOBILE_SPIKES,
    LAUNDROMOBILE_6,
}LaundroMobileTypes;

// Object Class
struct ObjectLaundroMobile {
    RSDK_OBJECT
    uint8 health;
    uint8 invincibilityTimer;
    int32 currentVelocity;
    Entity *loopPoints[6];
    uint8 loopPointCount;
    bool32 playingFanSfx;
    uint8 shouldPlayFanSfx;
    uint8 attackDir;
    int8 attackDelay;
    TABLE(int32 attackDelays[8], { 1, 2, 2, 2, 1, 2, 2, 3 });
    bool32 underwaterFlag;
    int32 animSpeed;
    bool32 rocketActive;
    int32 rocketAngles[4];
    Vector2 rocketPositions[4];
    uint8 value16[4];
    int32 rocketSpeeds[2];
    int32 playerRadius[4];
    int32 playerAngles[4];
    uint8 travelledPaths;
    bool32 useStageWrap;
    Entity *bossPtr;
    TABLE(int32 debrisInfo[73], { 12,       6,        0, -0x50000, -0x40000, 0, 0,        7,        0, 0x50000,  -0x40000, 0, 0,        10,       0,
                             -0x50000, -0x40000, 0, 0,        11,       0, 0x50000,  -0x40000, 0, 0,        10,       0, -0x50000, -0x40000, 0,
                             0x200000, 11,       0, 0x50000,  -0x40000, 0, 0x200000, 10,       0, -0x50000, -0x40000, 0, 0x400000, 11,       0,
                             0x50000,  -0x40000, 0, 0x400000, 8,        0, -0x50000, -0x40000, 0, 0x400000, 9,        0, 0x50000,  -0x40000, 0,
                             0x400000, 12,       0, -0x50000, -0x40000, 0, 0x400000, 13,       0, 0x50000,  -0x40000, 0, 0x400000 });
    uint16 aniFrames;
    uint16 eggmanFrames;
    Hitbox hitboxBoss;
    Hitbox hitboxMissile;
    Hitbox hitboxBox;
    Hitbox hitboxBomb;
    Hitbox innerBox;
    Hitbox outerBox;
    uint16 sfxHit;
    uint16 sfxExplosion;
    uint16 sfxButton2;
    uint16 sfxFan;
    uint16 sfxRoll;
    uint16 sfxWash;
    uint16 sfxHullClose;
    uint16 sfxPush;
    uint16 sfxFireball;
    uint16 sfxButton;
    uint16 sfxLedgeBreak;
    uint16 sfxPimPom;
#if RETRO_USE_PLUS
    uint16 sfxRumble;
    uint16 sfxImpact;
#endif
};

// Entity Class
struct EntityLaundroMobile {
    RSDK_ENTITY
    uint8 type;
    StateMachine(state);
    StateMachine(stateDraw);
    int32 timer;
    int32 startY;
    Vector2 startPos;
    Vector2 unknownPos;
    Entity *whirlpool;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
};

// Object Struct
extern ObjectLaundroMobile *LaundroMobile;

// Standard Entity Events
void LaundroMobile_Update(void);
void LaundroMobile_LateUpdate(void);
void LaundroMobile_StaticUpdate(void);
void LaundroMobile_Draw(void);
void LaundroMobile_Create(void *data);
void LaundroMobile_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void LaundroMobile_EditorDraw(void);
void LaundroMobile_EditorLoad(void);
#endif
void LaundroMobile_Serialize(void);

// Extra Entity Functions
void LaundroMobile_CheckPlayerCollisions(void);
void LaundroMobile_Explode(void);
void LaundroMobile_HandleStageWrap(void);
void LaundroMobile_HandleMissileMovement(void);
void LaundroMobile_HandleEggmanAnimations(void);
void LaundroMobile_HandleTileCollisions(void);

void LaundroMobile_State_SetupArena(void);
void LaundroMobile_State_SetupArena2(void);
void LaundroMobile_State_Unknown1(void);
void LaundroMobile_State_Unknown2(void);
void LaundroMobile_State_Unknown3(void);
void LaundroMobile_State_Destroyed_Phase1(void);

void LaundroMobile_State_Unknown4(void);
void LaundroMobile_State_Unknown5(void);
void LaundroMobile_State_Unknown6(void);
void LaundroMobile_State_Unknown7(void);
void LaundroMobile_State_Unknown8(void);
void LaundroMobile_State_Unknown9(void);
void LaundroMobile_State_Unknown11(void);
void LaundroMobile_State_Unknown12(void);
void LaundroMobile_State_Unknown13(void);
void LaundroMobile_State_Unknown14(void);
void LaundroMobile_State_Unknown15(void);
void LaundroMobile_State_Unknown16(void);
void LaundroMobile_State_Unknown17(void);
void LaundroMobile_State_Unknown18(void);
void LaundroMobile_State_Destroyed_Phase2(void);
void LaundroMobile_State_Finish(void);

#if RETRO_USE_PLUS
void LaundroMobile_StageFinishCB_Blank(void);

void LaundroMobile_State_StartOutro(void);
void LaundroMobile_State_OutroRumble(void);
void LaundroMobile_State_OutroWaterGush(void);
void LaundroMobile_State_ExitHCZ(void);
#endif

void LaundroMobile_StateDraw_Unknown1(void);
void LaundroMobile_StateDraw_Unknown2(void);

void LaundroMobile_State1_Unknown1(void);
void LaundroMobile_State1_Unknown2(void);
void LaundroMobile_State1_Unknown3(void);

void LaundroMobile_StateDraw4_Unknown1(void);

void LaundroMobile_State3_Unknown1(void);
void LaundroMobile_State3_Unknown2(void);

void LaundroMobile_State2_Unknown1(void);
void LaundroMobile_StateDraw2_Unknown1(void);

void LaundroMobile_State6_Unknown1(void);

#endif //! OBJ_LAUNDROMOBILE_H
