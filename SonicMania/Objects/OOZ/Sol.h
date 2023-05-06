#ifndef OBJ_SOL_H
#define OBJ_SOL_H

#include "Game.h"

#define SOL_FLAMEORB_COUNT (4)

// Object Class
struct ObjectSol {
    RSDK_OBJECT
    Hitbox hitboxBadnik;
    Hitbox hitboxOrb;
    uint16 aniFrames;
};

// Entity Class
struct EntitySol {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 startPos;
    uint8 startDir;
    uint8 activeOrbs;
    Vector2 positions[SOL_FLAMEORB_COUNT];
    int32 oscillateAngle;
    bool32 isFlameFX;
    bool32 fireOrbs;
    Animator mainAnimator;
    Animator ballAnimator;
};

// Object Struct
extern ObjectSol *Sol;

// Standard Entity Events
void Sol_Update(void);
void Sol_LateUpdate(void);
void Sol_StaticUpdate(void);
void Sol_Draw(void);
void Sol_Create(void *data);
void Sol_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Sol_EditorDraw(void);
void Sol_EditorLoad(void);
#endif
void Sol_Serialize(void);

// Extra Entity Functions
void Sol_DebugSpawn(void);
void Sol_DebugDraw(void);

void Sol_HandlePlayerInteractions(void);
void Sol_HandlePlayerHurt(void);
void Sol_HandleRotation(void);
void Sol_CheckOffScreen(void);

void Sol_State_Init(void);
void Sol_State_Moving(void);
void Sol_State_ShootingOrbs(void);
void Sol_State_NoOrbs(void);

void Sol_State_SmallFireball(void);
void Sol_State_ActiveFireball(void);

void Sol_State_FlameDissipate(void);
void Sol_State_FireballOilFlame(void);
void Sol_State_OilFlame(void);

#endif //! OBJ_SOL_H
