#ifndef OBJ_ERZKING_H
#define OBJ_ERZKING_H

#include "Game.h"

typedef enum {
    ERZKING_KING,
    ERZKING_ARM_L,
    ERZKING_ARM_R,
} ERZKingTypes;

typedef enum {
    ERZKING_HEAVY_GUNNER,
    ERZKING_HEAVY_MYSTIC,
} ERZKingHeavyTypes;

// Object Class
struct ObjectERZKing {
    RSDK_OBJECT
    int32 boundsL;
    int32 boundsR;
    int32 boundsM;
    int32 boundsT;
    int32 boundsB;
    uint16 sfxHit;
    uint16 sfxExplosion2;
    uint16 aniFrames;
};

// Entity Class
struct EntityERZKing {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    Vector2 originPos;
    int32 type;
    int32 timer;
    int32 nextType;
    int32 invincibilityTimer;
    int32 health;
    int32 typeChangeTimer;
    int32 bodyAngle;
    int32 armAngles[10];
    Vector2 armPositions[10];
    Vector2 rubyPos;
    Vector2 unused;
    EntityERZKing *parent;
    Animator basicAnimator;
    Animator headAnimator;
    Animator bodyAnimator;
    Animator beltAnimator;
    Animator particleAnimator;
    Animator rubyAnimator;
    Animator unusedAnimator;
    Animator armAnimator;
    Animator cuffAnimator;
    Animator handAnimator;
    Hitbox hitbox;
    uint8 storeDrawFX;
};

// Object Struct
extern ObjectERZKing *ERZKing;

// Standard Entity Events
void ERZKing_Update(void);
void ERZKing_LateUpdate(void);
void ERZKing_StaticUpdate(void);
void ERZKing_Draw(void);
void ERZKing_Create(void *data);
void ERZKing_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void ERZKing_EditorDraw(void);
void ERZKing_EditorLoad(void);
#endif
void ERZKing_Serialize(void);

// Extra Entity Functions
void ERZKing_CheckPlayerCollisions(void);
void ERZKing_Hit(void);
void ERZKing_HandleFrames(void);

void ERZKing_Draw_Body(void);
void ERZKing_Draw_Arm(void);

void ERZKing_State_SetupArena(void);
void ERZKing_State_SetupBody(void);
void ERZKing_State_EnterKing(void);
void ERZKing_State_FlyAround(void);
void ERZKing_State_ChangeHBH(void);
void ERZKing_State_Arm(void);
void ERZKing_State_Explode(void);

#endif //! OBJ_ERZKING_H
