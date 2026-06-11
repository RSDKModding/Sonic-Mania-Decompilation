#ifndef OBJ_TUBESPRING_H
#define OBJ_TUBESPRING_H

#include "Game.h"
#include "Global/Spring.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxExit;
    Hitbox hitbox;
} ObjectTubeSpring;

// Entity Class
typedef struct {
    // "Inherits" Spring
    RSDK_ENTITY
    StateMachine(state);
    SpringTypes type;
    FlipFlags flipFlag;
    uint8 planeFilter;
    int32 timer;
    Animator animator;
    Hitbox hitbox;
} EntityTubeSpring;

// Object Struct
extern ObjectTubeSpring *TubeSpring;

// Standard Entity Events
void TubeSpring_Update(void);
void TubeSpring_LateUpdate(void);
void TubeSpring_StaticUpdate(void);
void TubeSpring_Draw(void);
void TubeSpring_Create(void *data);
void TubeSpring_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void TubeSpring_EditorDraw(void);
void TubeSpring_EditorLoad(void);
#endif
void TubeSpring_Serialize(void);

// Extra Entity Functions
void TubeSpring_State_Idle(void);
void TubeSpring_State_Springing(void);
void TubeSpring_State_Pullback(void);

bool32 TubeSpring_HandleInteractions(bool32 setState);

#endif //! OBJ_TUBESPRING_H
