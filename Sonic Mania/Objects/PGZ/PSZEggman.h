#ifndef OBJ_PSZEGGMAN_H
#define OBJ_PSZEGGMAN_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 controlFrames;
    uint16 aniFrames;
} ObjectPSZEggman;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    Vector2 drawPos;
    EntityPhantomRuby *ruby;
    Animator animator;
    Animator animator2;
    Animator animator3;
    Animator animator4;
} EntityPSZEggman;

// Object Struct
extern ObjectPSZEggman *PSZEggman;

// Standard Entity Events
void PSZEggman_Update(void);
void PSZEggman_LateUpdate(void);
void PSZEggman_StaticUpdate(void);
void PSZEggman_Draw(void);
void PSZEggman_Create(void* data);
void PSZEggman_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void PSZEggman_EditorDraw(void);
void PSZEggman_EditorLoad(void);
#endif
void PSZEggman_Serialize(void);

// Extra Entity Functions
void PSZEggman_Unknown1(void);


#endif //!OBJ_PSZEGGMAN_H
