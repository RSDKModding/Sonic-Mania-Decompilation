#ifndef OBJ_PSZEGGMAN_H
#define OBJ_PSZEGGMAN_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort controlFrames;
    ushort aniFrames;
} ObjectPSZEggman;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int timer;
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
void PSZEggman_EditorDraw(void);
void PSZEggman_EditorLoad(void);
void PSZEggman_Serialize(void);

// Extra Entity Functions
void PSZEggman_Unknown1(void);


#endif //!OBJ_PSZEGGMAN_H
