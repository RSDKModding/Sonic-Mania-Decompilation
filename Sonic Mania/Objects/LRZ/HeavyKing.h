#ifndef OBJ_HEAVYKING_H
#define OBJ_HEAVYKING_H

#include "SonicMania.h"

// Object Class
struct ObjectHeavyKing {
    RSDK_OBJECT
    TABLE(int32 attackPattern[32], { 0, 0, 1, 0, 2, 2, 1, 0, 0, 1, 1, 2, 0, 2, 0, 0, 2, 1, 1, 2, 1, 0, 1, 0, 2, 2, 0, 1, 0, 0, 2, 1 });
    int32 attackPatternPos;
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
    int32 boundsL;
    int32 boundsM;
    int32 boundsR;
    int32 value9;
    int32 value10;
    uint16 sfxHit;
    uint16 sfxImpact2;
    uint16 sfxImpact5;
    uint16 sfxCharge;
    uint16 sfxExplosion;
    uint16 sfxFreeze;
    uint16 sfxRodPlant;
    uint16 sfxRodShine;
    uint16 sfxElecOn;
    uint16 sfxTwinCharge;
    uint16 sfxImpact6;
    uint16 aniFrames;
    uint16 cutsceneFrames;
};

// Entity Class
struct EntityHeavyKing {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateStore);
    int32 timer;
    int32 invincibilityTimer;
    int32 health;
    int32 field_6C;
    int32 field_70;
    Vector2 originPos;
    Entity *claw;
    Entity *masterEmerald;
    Entity *targetEmerald;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator1Store;
    Animator animator2Store;
};

// Object Struct
extern ObjectHeavyKing *HeavyKing;

// Standard Entity Events
void HeavyKing_Update(void);
void HeavyKing_LateUpdate(void);
void HeavyKing_StaticUpdate(void);
void HeavyKing_Draw(void);
void HeavyKing_Create(void* data);
void HeavyKing_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void HeavyKing_EditorDraw(void);
void HeavyKing_EditorLoad(void);
#endif
void HeavyKing_Serialize(void);

// Extra Entity Functions
void HeavyKing_CheckPlayerCollisions(void);
void HeavyKing_CheckPlayerCollisions_Charging(void);
void HeavyKing_Hit(void);
void HeavyKing_HandleClawMovement(void);
void HeavyKing_HandleAnimators(void);
void HeavyKing_Unknown6(void);
void HeavyKing_FindTargetEmerald(void);

void HeavyKing_Unknown8(int frame);
void HeavyKing_Unknown9(void);
void HeavyKing_CreateSpinRay(void);
void HeavyKing_CreateExpandRing(void);
void HeavyKing_CreateAttack(void);
void HeavyKing_CreateAttackCircle(void);

void HeavyKing_State_SetupArena(void);
void HeavyKing_State_HandleCutsceneSetup(void);
void HeavyKing_State_CutsceneUnknown1(void);
void HeavyKing_State_CutsceneUnknown2(void);
void HeavyKing_State_CutsceneUnknown3(void);
void HeavyKing_State_CutsceneUnknown4(void);
void HeavyKing_State_CutsceneUnknown5(void);
void HeavyKing_State_CutsceneUnknown6(void);
void HeavyKing_State_CutsceneUnknown7(void);
void HeavyKing_State_CutsceneUnknown8(void);
void HeavyKing_State_CutsceneUnknown9(void);
void HeavyKing_State_CutsceneUnknown10(void);

void HeavyKing_State_Unknown13(void);
void HeavyKing_State_Unknown1(void);
void HeavyKing_State_Unknown2(void);
void HeavyKing_State_Unknown3(void);
void HeavyKing_State_Unknown4(void);
void HeavyKing_State_Unknown5(void);
void HeavyKing_State_Unknown6(void);
void HeavyKing_State_Unknown7(void);
void HeavyKing_State_Unknown8(void);
void HeavyKing_State_Unknown9(void);
void HeavyKing_State_Unknown10(void);
void HeavyKing_State_Unknown11(void);
void HeavyKing_State_Unknown12(void);
void HeavyKing_State_HitRecoil(void);
void HeavyKing_State_Destroyed(void);
void HeavyKing_State_Escape(void);
void HeavyKing_State_Finish(void);

#endif //!OBJ_HEAVYKING_H
