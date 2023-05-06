#ifndef OBJ_FERNPARALLAX_H
#define OBJ_FERNPARALLAX_H

#include "Game.h"

#if MANIA_USE_PLUS
// Object Class
struct ObjectFernParallax {
    RSDK_OBJECT
    uint16 aniFrames;
    uint8 entityBuffer[2 * ENTITY_SIZE];
};

// Entity Class
struct EntityFernParallax {
    RSDK_ENTITY
    StateMachine(state); // unused
    Animator animator;
    uint8 aniID;
    Vector2 parallaxFactor;
};

// Object Struct
extern ObjectFernParallax *FernParallax;

// Standard Entity Events
void FernParallax_Update(void);
void FernParallax_LateUpdate(void);
void FernParallax_StaticUpdate(void);
void FernParallax_Draw(void);
void FernParallax_Create(void *data);
void FernParallax_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void FernParallax_EditorDraw(void);
void FernParallax_EditorLoad(void);
#endif
void FernParallax_Serialize(void);

// Extra Entity Functions
#endif

#endif //! OBJ_FERNPARALLAX_H
