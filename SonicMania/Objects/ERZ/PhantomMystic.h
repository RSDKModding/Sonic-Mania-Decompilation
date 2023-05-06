#ifndef OBJ_PHANTOMMYSTIC_H
#define OBJ_PHANTOMMYSTIC_H

#include "Game.h"

// Object Class
struct ObjectPhantomMystic {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxCupSwap;
    uint16 sfxBigLaser;
    uint16 sfxImpact;
};

// Entity Class
struct EntityPhantomMystic {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    Vector2 originPos;
    Vector2 mysticPos;
    int32 mysticVelY;
    int32 middleCupY;
    int32 timer;
    int32 invincibilityTimer;
    int32 correctCup;
    int32 cupPos[3];
    int32 unused1[3];
    int32 unused2[3];
    int32 cupAlpha[3];
    int32 swapCup1;
    int32 swapCup2;
    int32 swapCup1Pos;
    int32 swapCup2Pos;
    int32 swapCup1Alpha;
    int32 swapCup2Alpha;
    int32 cupSwapCount;
    Animator mysticAnimator;
    Animator cupAnimator;
    Animator cupSilhouetteAnimator;
    Animator cupSpikeAnimator; // Not actually ever drawn...
    Animator cupBlastAnimator;
    Hitbox hitbox;
};

// Object Struct
extern ObjectPhantomMystic *PhantomMystic;

// Standard Entity Events
void PhantomMystic_Update(void);
void PhantomMystic_LateUpdate(void);
void PhantomMystic_StaticUpdate(void);
void PhantomMystic_Draw(void);
void PhantomMystic_Create(void *data);
void PhantomMystic_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PhantomMystic_EditorDraw(void);
void PhantomMystic_EditorLoad(void);
#endif
void PhantomMystic_Serialize(void);

// Extra Entity Functions
void PhantomMystic_CheckPlayerCollisions(void);
void PhantomMystic_Hit(void);
void PhantomMystic_SetupNewCupSwap(void);

void PhantomMystic_Draw_CupSetup(void);
void PhantomMystic_Draw_CupSwap(void);

void PhantomMystic_State_Init(void);
void PhantomMystic_State_SetupInitialCupPos(void);
void PhantomMystic_State_MoveCupsDownwards(void);
void PhantomMystic_State_RotateMiddleCup(void);
void PhantomMystic_State_MoveMiddleCupToFloor(void);
void PhantomMystic_State_PrepareCupSwap(void);
void PhantomMystic_State_CupSwapping(void);
void PhantomMystic_State_RevealMystic(void);
void PhantomMystic_State_CupBlast(void);
void PhantomMystic_State_MoveCupsToMystic(void);

#endif //! OBJ_PHANTOMMYSTIC_H
