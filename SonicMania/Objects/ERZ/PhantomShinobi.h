#ifndef OBJ_PHANTOMSHINOBI_H
#define OBJ_PHANTOMSHINOBI_H

#include "Game.h"

// Object Class
struct ObjectPhantomShinobi {
    RSDK_OBJECT
    Hitbox hitbox;
    uint16 aniFrames;
    uint16 sfxBlade;
    uint16 sfxHit;
};

// Entity Class
struct EntityPhantomShinobi {
    RSDK_ENTITY
    StateMachine(state);
    Vector2 startPos;
    int32 timer;
    int32 invincibilityTimer;
    int32 unused;
    int32 finRadius;
    int32 rotStore;
    int32 rotSpeed;
    int32 numBounces;
    int32 attackCount;
    bool32 prevOnGround;
    Hitbox outerBox;
    Hitbox innerBox;
    Animator bodyAnimator;
    Animator armAnimator;
    Animator finAnimator;
};

// Object Struct
extern ObjectPhantomShinobi *PhantomShinobi;

// Standard Entity Events
void PhantomShinobi_Update(void);
void PhantomShinobi_LateUpdate(void);
void PhantomShinobi_StaticUpdate(void);
void PhantomShinobi_Draw(void);
void PhantomShinobi_Create(void *data);
void PhantomShinobi_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PhantomShinobi_EditorDraw(void);
void PhantomShinobi_EditorLoad(void);
#endif
void PhantomShinobi_Serialize(void);

// Extra Entity Functions
void PhantomShinobi_CheckPlayerCollisions(void);
void PhantomShinobi_Hit(void);
void PhantomShinobi_HandleTileCollisions(void);
void PhantomShinobi_ResetStates(void);

void PhantomShinobi_State_EnterShinobi(void);
void PhantomShinobi_State_AttackDelay(void);
void PhantomShinobi_State_SetupAttack(void);
void PhantomShinobi_State_Moving(void);
void PhantomShinobi_State_PrepareFinAttack(void);
void PhantomShinobi_State_ExtendFins(void);
void PhantomShinobi_State_RetractFins(void);
void PhantomShinobi_State_FinishedAttack(void);

bool32 PhantomShinobi_SfxCheck_ShinobiBlade(void);

#endif //! OBJ_PHANTOMSHINOBI_H
