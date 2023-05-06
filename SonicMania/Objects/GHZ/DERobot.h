#ifndef OBJ_DEROBOT_H
#define OBJ_DEROBOT_H

#include "Game.h"

typedef enum {
    DEROBOT_BODY,
    DEROBOT_HEAD,
    DEROBOT_ARM,
    DEROBOT_LEG,
    DEROBOT_SPIKES,
    DEROBOT_ARM_THREAD,
    DEROBOT_TARGET_EDGE,
    DEROBOT_TARGET_CENTER,
    DEROBOT_TARGET_LOCK,
    DEROBOT_BOMB,
    DEROBOT_BODY_CUTSCENE,
} DERobotAniLists;

// Object Class
struct ObjectDERobot {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitboxHand;
    Hitbox hitboxBody;
    uint16 unused1;
    uint16 unused2;
    uint16 unused3;
    uint16 unused4;
    uint16 sfxHit;
    uint16 sfxExplosion;
    uint16 sfxTargeting;
    uint16 sfxImpact;
    uint16 sfxLedgeBreak;
    uint16 sfxBuzzsaw;
    uint16 sfxDrop;
    uint16 sfxButton2;
    uint16 sfxHullClose;
};

// Entity Class
struct EntityDERobot {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int32 unused1;
    int32 unused2;
    int32 unused3;
    int32 unused4;
    Entity *parent;
    EntityDERobot *head;
    EntityDERobot *shoulderFront;
    EntityEggman *eggman;
    EntityDERobot *legs[6]; // knee, leg, foot (for back, then for front)
    EntityDERobot *arms[4]; // arm, hand, (for back, then for front)
    int32 movingSide;
    int32 unused5;
    int32 angleVel;
    int32 angleInc;
    Vector2 offset;
    int32 timer;
    int32 invincibilityTimer;
    int32 health;
    int32 armExtend;
    int32 destroyedTerrainX;
    int32 kneeAngleVel;
    int32 angleVelStart;
    int32 angleMaxInc;
    Hitbox hitbox;
    Animator mainAnimator;
    Animator altAnimator;
    Animator armAnimator;
    int32 aniID;
    int32 frameID;
};

// Object Struct
extern ObjectDERobot *DERobot;

// Standard Entity Events
void DERobot_Update(void);
void DERobot_LateUpdate(void);
void DERobot_StaticUpdate(void);
void DERobot_Draw(void);
void DERobot_Create(void *data);
void DERobot_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void DERobot_EditorDraw(void);
void DERobot_EditorLoad(void);
#endif
void DERobot_Serialize(void);

// Extra Entity Functions
void DERobot_HandleScreenBounds(void);
void DERobot_HandleLegMovement(int32 offset);
void DERobot_HandleLegMovement2(int32 offset);
void DERobot_HandleArmMovement(int32 offset);
void DERobot_HandleTerrainDestruction(void);
void DERobot_DestroyTerrainFinal(void);
void DERobot_Hit(void);
void DERobot_Explode(void);

void DERobot_CheckPlayerCollisions_Body(void);
void DERobot_CheckPlayerCollisions_ArmExtend(void);
void DERobot_CheckPlayerCollisions_Hand(void);
bool32 DERobot_CheckRubyGrabbed(void);
void DERobot_CheckPlayerCollisions_Bomb(void);

void DERobot_Draw_RelativeToParent(void);
void DERobot_Draw_Arm(void);
void DERobot_Draw_Simple(void);
void DERobot_Draw_FrontLeg(void);
void DERobot_Draw_Target(void);

void DERobot_State_ArmIdle(void);
void DERobot_State_ArmExtendPrepare(void);
void DERobot_State_ArmExtending(void);
void DERobot_State_ArmRetracting(void);
void DERobot_State_ArmDestroyed(void);

void DERobot_Cutscene_ActivateArm(void);
void DERobot_Cutscene_ReachForRuby(void);
void DERobot_Cutscene_GrabbedRuby(void);
void DERobot_Cutscene_ArmDeactivate(void);

void DERobot_State_CloseHeadHatch(void);
void DERobot_State_BombLaunched(void);
void DERobot_State_BombLanded(void);
void DERobot_State_BombExplode(void);

void DERobot_State_SetupArena(void);
void DERobot_State_SetupBoss(void);

void DERobot_State_Target(void);

void DERobot_State_SurpriseFall(void);
void DERobot_State_FallLand(void);
void DERobot_State_Walk(void);
void DERobot_State_ArmAttack(void);
void DERobot_State_Explode(void);
void DERobot_State_ExplodeTerrain(void);
void DERobot_State_Finish(void);

void DERobot_State_DebrisFall(void);

void DERobot_State_FinishBounds(void);
void DERobot_State_CutsceneExplode(void);

#endif //! OBJ_DEROBOT_H
