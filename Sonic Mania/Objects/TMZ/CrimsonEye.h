#ifndef OBJ_CRIMSONEYE_H
#define OBJ_CRIMSONEYE_H

#include "SonicMania.h"

typedef enum {
    CRIMSONEYE_CONATINER,
    CRIMSONEYE_CORE,
    CRIMSONEYE_BALL,
    CRIMSONEYE_ARROW,
    CRIMSONEYE_SPIKE,
    CRIMSONEYE_SHOT,
} CrimsonEyeTypes;

typedef enum {
    CE_ARROW_DOWN = 1,
    CE_ARROW_UP = 4,
    CE_ARROW_OFF  = 0xFF,
} CrimsonEyeArrowTypes;

// Object Class
typedef struct {
    RSDK_OBJECT
    TABLE(int32 debrisInfo[97],
          { 24,       0,  0, -0x28000, -0x28000, 1,  0, -0x14000, -0x28000, 2,  0, 0x14000,  -0x26000, 3,  0, 0x28000,  -0x26000, 4,  0, -0x26000,
            -0x24000, 5,  0, 0x26000,  -0x24000, 6,  0, -0x26000, -0x22000, 7,  0, 0x26000,  -0x22000, 8,  0, -0x24000, -0x20000, 9,  0, -0x12000,
            -0x20000, 10, 0, 0x12000,  -0x20000, 11, 0, 0x24000,  -0x20000, 12, 0, -0x22000, -0x1E000, 13, 0, -0x11000, -0x1E000, 14, 0, 0x11000,
            -0x1E000, 15, 0, 0x22000,  -0x1E000, 16, 0, -0x20000, -0x1C000, 17, 0, -0x10000, -0x1C000, 18, 0, 0x10000,  -0x1C000, 19, 0, 0x20000,
            -0x1C000, 20, 0, -0x1A000, -0x1A000, 21, 0, -0xD000,  -0x1A000, 22, 0, 0xD000,   -0x1A000, 23, 0, 0x1A000,  -0x1A000 });
    uint8 health;
    uint8 invincibilityTimer;
    uint8 value4;
    uint8 value5;
    int32 value6;
    int32 value7;
    Vector2 value8;
    Vector2 positions[3];
    TABLE(int32 ballDestroyOrder[8], { 1, 5, 2, 6, 3, 7, 4, 8 });
    int32 destroyedBallCount;
    int32 scrollPos;
    int32 scrollLimit;
    int32 value14;
    int32 value15;
    int32 value16;
    uint8 value17;
    int32 value18;
    uint16 aniFrames;
    Hitbox hitboxEye;
    Hitbox hitbox2;
    Hitbox hitboxOrb;
    Hitbox hitboxBlock;
    Hitbox hitboxElecOrb;
    Hitbox hitbox6;
    Hitbox hitbox7;
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
    TileLayer *liftBG;
    TileLayer *layerBG1;
} ObjectCrimsonEye;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateEye);
    StateMachine(stateDraw);
    int32 type;
    int32 timer;
    int32 timer2;
    int32 field_70;
    Vector2 field_74;
    Animator animator;
} EntityCrimsonEye;

// Object Struct
extern ObjectCrimsonEye *CrimsonEye;

// Standard Entity Events
void CrimsonEye_Update(void);
void CrimsonEye_LateUpdate(void);
void CrimsonEye_StaticUpdate(void);
void CrimsonEye_Draw(void);
void CrimsonEye_Create(void* data);
void CrimsonEye_StageLoad(void);
void CrimsonEye_EditorDraw(void);
void CrimsonEye_EditorLoad(void);
void CrimsonEye_Serialize(void);

// Extra Entity Functions
void CrimsonEye_Explode(void);

void CrimsonEye_HandleBGMovement(void);
void CrimsonEye_DrawLayerCB_B(void);
void CrimsonEye_DrawLayerCB_A(void);

void CrimsonEye_SetArrowDir(int type);

void CrimsonEye_StateBody_Unknown1(void);
void CrimsonEye_StateBody_Unknown2(void);
void CrimsonEye_StateBody_Unknown3(void);
void CrimsonEye_StateBody_Unknown4(void);
void CrimsonEye_StateBody_Unknown5(void);
void CrimsonEye_StateBody_Explode(void);
void CrimsonEye_StateBody_Unknown6(void);
void CrimsonEye_StateDraw_Container(void);

void CrimsonEye_StateEye_Unknown1(void);
void CrimsonEye_StateEye_Unknown2(void);
void CrimsonEye_StateEye_Unknown3(void);
void CrimsonEye_StateEye_Unknown4(void);
void CrimsonEye_StateEye_Unknown5(void);
void CrimsonEye_StateEye_Unknown6(void);
void CrimsonEye_StateEye_Unknown7(void);

void CrimsonEye_ShootShot(void);
void CrimsonEye_SetupPositions(void);
void CrimsonEye_DestroyBall(void);
void CrimsonEye_CheckPlayerCollisions(void);

void CrimsonEye_StateCore_Unknown1(void);
void CrimsonEye_StateCore_BreakOut(void);
void CrimsonEye_StateCore_Unknown3(void);
void CrimsonEye_StateCore_Unknown4(void);
void CrimsonEye_StateCore_Unknown5(void);
void CrimsonEye_StateCore_Explode(void);
void CrimsonEye_StateCore_SpawnSignPost(void);
void CrimsonEye_StateDraw_Core(void);

void CrimsonEye_CheckPlayerCollisions_Ball(void);
void CrimsonEye_StateBall_Unknown1(void);
void CrimsonEye_StateBall_Unknown2(void);

void CrimsonEye_StateDraw_Simple(void);

void CrimsonEye_StateSpike_CheckPlayerCollisions(void);
void CrimsonEye_StateSpike_Move(void);

void CrimsonEye_StateShot_CheckPlayerCollisions(void);

void CrimsonEye_StateArrow(void);
void CrimsonEye_StateDraw_Arrow(void);

#endif //!OBJ_CRIMSONEYE_H
