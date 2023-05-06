#ifndef OBJ_SELTZERBOTTLE_H
#define OBJ_SELTZERBOTTLE_H

#include "Game.h"

// Object Class
struct ObjectSeltzerBottle {
    RSDK_OBJECT
    Hitbox hitboxBottle;
    Hitbox hitboxButton;
    uint16 seltzerPathLayer;
    uint16 seltzerPathLayerMask;
    uint16 aniFrames;
    uint16 sfxSpray;
};

// Entity Class
struct EntitySeltzerBottle {
    RSDK_ENTITY
    StateMachine(state);
    int32 sprayTime;
    int32 timer;
    int32 waterLevel;
    Animator unusedAnimator;
    Animator bottleAnimator;
    Animator buttonAnimator;
    Animator waterAnimator;
    Animator sprayAnimator;
};

// Object Struct
extern ObjectSeltzerBottle *SeltzerBottle;

// Standard Entity Events
void SeltzerBottle_Update(void);
void SeltzerBottle_LateUpdate(void);
void SeltzerBottle_StaticUpdate(void);
void SeltzerBottle_Draw(void);
void SeltzerBottle_Create(void *data);
void SeltzerBottle_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void SeltzerBottle_EditorDraw(void);
void SeltzerBottle_EditorLoad(void);
#endif
void SeltzerBottle_Serialize(void);

// Extra Entity Functions
void SeltzerBottle_State_Spraying(void);
void SeltzerBottle_State_TryReset(void);

#endif //! OBJ_SELTZERBOTTLE_H
