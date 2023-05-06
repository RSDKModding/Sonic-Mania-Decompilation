#ifndef OBJ_CRIMSONEYE_H
#define OBJ_CRIMSONEYE_H

#include "Game.h"

typedef enum {
    CRIMSONEYE_CONTAINER,
    CRIMSONEYE_CORE,
    CRIMSONEYE_BALL,
    CRIMSONEYE_ARROW,
    CRIMSONEYE_SPIKE,
    CRIMSONEYE_SHOT,
} CrimsonEyeTypes;

typedef enum {
    CE_ARROW_DOWN = 1,
    CE_ARROW_UP   = 4,
    CE_ARROW_OFF  = 0xFF,
} CrimsonEyeArrowTypes;

// Object Class
struct ObjectCrimsonEye {
    RSDK_OBJECT
    TABLE(int32 debrisInfo[97],
          { 24,       0,  0, -0x28000, -0x28000, 1,  0, -0x14000, -0x28000, 2,  0, 0x14000,  -0x26000, 3,  0, 0x28000,  -0x26000, 4,  0, -0x26000,
            -0x24000, 5,  0, 0x26000,  -0x24000, 6,  0, -0x26000, -0x22000, 7,  0, 0x26000,  -0x22000, 8,  0, -0x24000, -0x20000, 9,  0, -0x12000,
            -0x20000, 10, 0, 0x12000,  -0x20000, 11, 0, 0x24000,  -0x20000, 12, 0, -0x22000, -0x1E000, 13, 0, -0x11000, -0x1E000, 14, 0, 0x11000,
            -0x1E000, 15, 0, 0x22000,  -0x1E000, 16, 0, -0x20000, -0x1C000, 17, 0, -0x10000, -0x1C000, 18, 0, 0x10000,  -0x1C000, 19, 0, 0x20000,
            -0x1C000, 20, 0, -0x1A000, -0x1A000, 21, 0, -0xD000,  -0x1A000, 22, 0, 0xD000,   -0x1A000, 23, 0, 0x1A000,  -0x1A000 });
    uint8 health;
    uint8 invincibilityTimer;
    uint8 ballSpinAngleX;
    uint8 ballSpinAngleY;
    int32 ballSpinRadius;
    int32 ballOffsetY;
    Vector2 containerPos;
    Vector2 eyePositions[3];
    TABLE(int32 ballDestroyOrder[8], { 1, 5, 2, 6, 3, 7, 4, 8 });
    int32 destroyedBallCount;
    int32 elevatorSpeed;
    int32 targetElevatorSpeed;
    int32 nextTargetElevatorSpeed;
    int32 elevatorGravity; // probably not this, but I really didn't have much to work with here, sorry!
    int32 shotTimer;
    uint8 shotsRemaining;
    int32 unused;
    uint16 aniFrames;
    Hitbox hitboxEye;
    Hitbox hitboxBall;
    Hitbox hitboxOrb;
    Hitbox hitboxBlock;
    Hitbox hitboxElecOrb;
    Hitbox hitboxUnused1;
    Hitbox hitboxUnused2;
    uint16 sfxHit;
    uint16 sfxExplosion;
    uint16 sfxHover;
    uint16 sfxHover2;
    uint16 sfxElevator;
    uint16 sfxShot;
    uint16 sfxBeep;
    uint16 sfxHullClose;
    uint16 sfxButton;
    uint16 sfxImpact;
    TileLayer *liftBackground;
    TileLayer *bg2Layer;
};

// Entity Class
struct EntityCrimsonEye {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateEye);
    StateMachine(stateDraw);
    int32 type;
    int32 timer;
    int32 ballTimer;
    int32 ballRadiusTimer;
    Vector2 originPos;
    Animator animator;
};

// Object Struct
extern ObjectCrimsonEye *CrimsonEye;

// Standard Entity Events
void CrimsonEye_Update(void);
void CrimsonEye_LateUpdate(void);
void CrimsonEye_StaticUpdate(void);
void CrimsonEye_Draw(void);
void CrimsonEye_Create(void *data);
void CrimsonEye_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void CrimsonEye_EditorDraw(void);
void CrimsonEye_EditorLoad(void);
#endif
void CrimsonEye_Serialize(void);

// Extra Entity Functions
void CrimsonEye_Explode(void);
void CrimsonEye_Hit(void);

void CrimsonEye_SetupBG2Layer(void);
void CrimsonEye_DrawHook_DisableFGSilhouette(void);
void CrimsonEye_DrawHook_EnableFGSilhouette(void);

void CrimsonEye_SetArrowDir(int32 type);

void CrimsonEye_StateContainer_SetupArena(void);
void CrimsonEye_StateContainer_AwaitPlayer(void);
void CrimsonEye_StateContainer_StartFight(void);
void CrimsonEye_StateContainer_CoreActive(void);
void CrimsonEye_StateContainer_HandleElevator(void);
void CrimsonEye_StateContainer_Explode(void);
void CrimsonEye_StateContainer_MoveElevatorToTMZ2Entry(void);
void CrimsonEye_Draw_Container(void);

void CrimsonEye_StateEye_EnterBalls(void);
void CrimsonEye_StateEye_SpinningBalls(void);
void CrimsonEye_StateEye_LowerBalls(void);
void CrimsonEye_StateEye_LoweredBalls(void);
void CrimsonEye_StateEye_ExtendBallAttack(void);
void CrimsonEye_StateEye_FinishExtendAttack(void);
void CrimsonEye_StateEye_RaiseBalls(void);

void CrimsonEye_ShootShot(void);
void CrimsonEye_SetupEyePositions(void);
void CrimsonEye_DestroyBall(void);
void CrimsonEye_CheckPlayerCollisions(void);

void CrimsonEye_StateCore_ContainerActive(void);
void CrimsonEye_StateCore_BreakOut(void);
void CrimsonEye_StateCore_BrokenOut(void);
void CrimsonEye_StateCore_Hovering(void);
void CrimsonEye_StateCore_ImpactLift(void);
void CrimsonEye_StateCore_Explode(void);
void CrimsonEye_StateCore_SpawnSignPost(void);
void CrimsonEye_Draw_Core(void);

void CrimsonEye_CheckPlayerCollisions_Ball(void);
void CrimsonEye_StateBall_Spinning(void);
void CrimsonEye_StateBall_Destroyed(void);

void CrimsonEye_Draw_Simple(void);

void CrimsonEye_StateSpike_Harmful(void);
void CrimsonEye_StateSpike_Debris(void);

void CrimsonEye_State_Shot(void);

void CrimsonEye_State_Arrow(void);
void CrimsonEye_Draw_Arrow(void);

#endif //! OBJ_CRIMSONEYE_H
