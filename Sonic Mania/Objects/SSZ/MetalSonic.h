#ifndef OBJ_METALSONIC_H
#define OBJ_METALSONIC_H

#include "SonicMania.h"

// Object Class
struct ObjectMetalSonic {
    RSDK_OBJECT 
    int32 field_4;
    int32 field_8;
    uint16 field_C;
    uint16 sfxHit;
    uint16 sfxExplosion2;
    uint16 sfxExplosion3;
    uint16 sfxRumble;
    uint16 sfxJump2;
    uint16 sfxSpecialRing;
    uint16 sfxMSElecPulse;
    uint16 sfxMSBall;
    uint16 sfxMSFireball;
    uint16 sfxBeep3;
    uint16 sfxBeep4;
    uint16 sfxRockemSockem;
    uint16 sfxMSShoot;
    uint16 sfxMSChargeFire;
#if RETRO_USE_PLUS
    uint16 sfxMSTransform;
    uint16 sfxTransform2;
#endif
    Hitbox hitbox1;
    Hitbox hitbox2;
    uint16 aniFrames;
};

// Entity Class
struct EntityMetalSonic {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 targetPos;
    int32 timer;
    int32 field_68;
    int32 field_6C;
    int32 health;
    int32 field_74;
    int32 field_78;
    int32 field_7C;
    int32 field_80;
    int32 field_84;
    int32 timer2;
    uint8 attackType;
    int32 field_90;
    int32 field_94;
    Entity* panel;
    Vector2 field_9C;
    Vector2 field_A4;
    Vector2 field_AC;
    Hitbox* outerBox;
    Hitbox* innerBox;
    Animator animator;
    Animator animator2;
};

// Object Struct
extern ObjectMetalSonic *MetalSonic;

// Standard Entity Events
void MetalSonic_Update(void);
void MetalSonic_LateUpdate(void);
void MetalSonic_StaticUpdate(void);
void MetalSonic_Draw(void);
void MetalSonic_Create(void* data);
void MetalSonic_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void MetalSonic_EditorDraw(void);
void MetalSonic_EditorLoad(void);
#endif
void MetalSonic_Serialize(void);

// Extra Entity Functions
void MetalSonic_HandleStageWrap(void);
void MetalSonic_ProcessBGParallax(int32 mult);
void MetalSonic_HandleVelocity(void);
void MetalSonic_HandleAnimDir(void);
#if !RETRO_USE_PLUS
void MetalSonic_CheckPlayerCollisions(void);
void MetalSonic_Hit(void);
void MetalSonic_Explode(void);
#endif

void MetalSonic_State_SetupArena(void);
void MetalSonic_State_WaitForPlayer(void);
void MetalSonic_State_WaitForHologram(void);
void MetalSonic_State_Appear(void);
void MetalSonic_State_Land(void);
void MetalSonic_State_Taunt(void);
void MetalSonic_State_GetReady(void);
void MetalSonic_State_SetupBounds(void);
void MetalSonic_State_Start(void);
void MetalSonic_State_Unknown1(void);
void MetalSonic_State_Unknown2(void);
void MetalSonic_State_Unknown3(void);
void MetalSonic_State_StartAttack(void);
void MetalSonic_State_Unknown5(void);
void MetalSonic_State_Unknown6(void);
void MetalSonic_State_Unknown7(void);
void MetalSonic_State_Unknown8(void);
void MetalSonic_State_Unknown9(void);
void MetalSonic_State_Unknown10(void);
void MetalSonic_State_EnterPanel(void);
void MetalSonic_State_StartPanelSeq(void);
void MetalSonic_HandlePanelAttack(void);
void MetalSonic_State_ShowFactory(void);
void MetalSonic_State_Unknown13(void);
void MetalSonic_State_PanelExplosion(void);
void MetalSonic_State_Unknown14(void);
void MetalSonic_State_Unknown15(void);

#if RETRO_USE_PLUS //Phase 3 (GigaMetal)
void MetalSonic_State_WaitForRuby(void);
void MetalSonic_State_ObtainRuby(void);
void MetalSonic_State_Transform(void);
void MetalSonic_State_Defeated(void);
#else // Phase 3 (VS Metal Sonic)
void MetalSonic_State_Unknown16(void);
void MetalSonic_State_Unknown17(void);
void MetalSonic_State_Unknown18(void);
void MetalSonic_State_Unknown19(void);
void MetalSonic_State_Unknown20(void);
void MetalSonic_State_Unknown21(void);
void MetalSonic_State_Unknown22(void);
void MetalSonic_State_Unknown23(void);
void MetalSonic_State_Unknown24(void);
void MetalSonic_State_Unknown25(void);
void MetalSonic_State_Unknown26(void);
void MetalSonic_State_Unknown27(void);
void MetalSonic_State_Unknown28(void);
void MetalSonic_State_Unknown29(void);
void MetalSonic_State_Unknown30(void);
void MetalSonic_State_Explode(void);
void MetalSonic_State_Defeated(void);
void MetalSonic_State_Finish(void);
void MetalSonic_State_None(void);

void MetalSonic_StateWall_Fall(void);
void MetalSonic_StateWall_Move(void);
#endif

#endif //!OBJ_METALSONIC_H
