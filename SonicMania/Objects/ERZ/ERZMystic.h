#ifndef OBJ_ERZMYSTIC_H
#define OBJ_ERZMYSTIC_H

#include "Game.h"

// Object Class
struct ObjectERZMystic {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityERZMystic {
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
extern ObjectERZMystic *ERZMystic;

// Standard Entity Events
void ERZMystic_Update(void);
void ERZMystic_LateUpdate(void);
void ERZMystic_StaticUpdate(void);
void ERZMystic_Draw(void);
void ERZMystic_Create(void *data);
void ERZMystic_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void ERZMystic_EditorDraw(void);
void ERZMystic_EditorLoad(void);
#endif
void ERZMystic_Serialize(void);

// Extra Entity Functions
void ERZMystic_CheckPlayerCollisions(void);
void ERZMystic_Hit(void);
void ERZMystic_SetupNewCupSwap(void);

void ERZMystic_Draw_CupSetup(void);
void ERZMystic_Draw_CupSwap(void);

void ERZMystic_State_Init(void);
void ERZMystic_State_SetupInitialCupPos(void);
void ERZMystic_State_MoveCupsDownwards(void);
void ERZMystic_State_RotateMiddleCup(void);
void ERZMystic_State_MoveMiddleCupToFloor(void);
void ERZMystic_State_PrepareCupSwap(void);
void ERZMystic_State_CupSwapping(void);
void ERZMystic_State_RevealMystic(void);
void ERZMystic_State_CupBlast(void);
void ERZMystic_State_MoveCupsToMystic(void);

#endif //! OBJ_ERZMYSTIC_H
