#ifndef OBJ_BLADEPOLE_H
#define OBJ_BLADEPOLE_H

#include "Game.h"

// Object Class
struct ObjectBladePole {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitboxTop;
    Hitbox hitboxBottom;
};

// Entity Class
struct EntityBladePole {
    RSDK_ENTITY
    StateMachine(state);
    uint16 timer;
    Animator poleAnimator;
    Animator bladeTopAnimator;
    Animator bladeBottomAnimator;
};

// Object Struct
extern ObjectBladePole *BladePole;

// Standard Entity Events
void BladePole_Update(void);
void BladePole_LateUpdate(void);
void BladePole_StaticUpdate(void);
void BladePole_Draw(void);
void BladePole_Create(void *data);
void BladePole_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void BladePole_EditorDraw(void);
void BladePole_EditorLoad(void);
#endif
void BladePole_Serialize(void);

// Extra Entity Functions
void BladePole_DrawSprites(void);

bool32 BladePole_SetAnimation(Animator *animator);
void BladePole_CheckPlayerCollisions(Hitbox *hitbox);

void BladePole_State_TopBladeActive(void);
void BladePole_State_BottomBladeActive(void);

#endif //! OBJ_BLADEPOLE_H
