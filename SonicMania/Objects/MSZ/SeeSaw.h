#ifndef OBJ_SEESAW_H
#define OBJ_SEESAW_H

#include "Game.h"

typedef enum {
    SEESAW_TILT_L,
    SEESAW_TILT_M,
    SEESAW_TILT_R,
} SeeSawTilts;

// Object Class
struct ObjectSeeSaw {
    RSDK_OBJECT
    TABLE(int32 tiltHeightTable[48],
          { -36, -36, -38, -40, -42, -44, -42, -40, -38, -36, -35, -34, -33, -32, -31, -30, -29, -28, -27, -26, -25, -24, -23, -22,
            -21, -20, -19, -18, -17, -16, -15, -14, -13, -12, -11, -10, -9,  -8,  -7,  -6,  -5,  -4,  -3,  -2,  -2,  -2,  -2,  -2 });
    Vector2 orbTargetPos;
    int32 launchVelocity;
    Hitbox hitboxPlank;
    Hitbox hitboxCactinaut;
    uint16 aniFrames;
    uint16 sfxSpring;
};

// Entity Class
struct EntitySeeSaw {
    RSDK_ENTITY
    StateMachine(state);
    uint8 side;
    Vector2 orbPos;
    int32 orbTimer; // unused... in mania, it's used in S1 & S2, which this object is based on
    int32 tiltTimerL;
    int32 tiltTimerM;
    int32 tiltTimerR;
    int32 tilt;
    int32 prevTilt;
    int32 targetTilt;
    int32 launchVelocity;
    int32 orbSide;
    bool32 stood;
    Animator pivotAnimator;
    Animator plankAnimator;
    Animator orbAnimator;
};

// Object Struct
extern ObjectSeeSaw *SeeSaw;

// Standard Entity Events
void SeeSaw_Update(void);
void SeeSaw_LateUpdate(void);
void SeeSaw_StaticUpdate(void);
void SeeSaw_Draw(void);
void SeeSaw_Create(void *data);
void SeeSaw_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void SeeSaw_EditorDraw(void);
void SeeSaw_EditorLoad(void);
#endif
void SeeSaw_Serialize(void);

// Extra Entity Functions
void SeeSaw_SetupHitbox(int32 playerX, int32 tilt);
void SeeSaw_State_OrbIdle(void);
void SeeSaw_State_None(void);
void SeeSaw_State_NoOrb(void);
void SeeSaw_State_OrbLaunched(void);

#endif //! OBJ_SEESAW_H
