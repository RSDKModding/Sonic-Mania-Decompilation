#ifndef OBJ_SSZEGGMAN_H
#define OBJ_SSZEGGMAN_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectSSZEggman;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int timer;
    Vector2 rubyPos;
    Vector2 offset;
    int speed;
    EntityPhantomRuby *ruby;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
} EntitySSZEggman;

// Object Struct
extern ObjectSSZEggman *SSZEggman;

// Standard Entity Events
void SSZEggman_Update(void);
void SSZEggman_LateUpdate(void);
void SSZEggman_StaticUpdate(void);
void SSZEggman_Draw(void);
void SSZEggman_Create(void* data);
void SSZEggman_StageLoad(void);
void SSZEggman_EditorDraw(void);
void SSZEggman_EditorLoad(void);
void SSZEggman_Serialize(void);

// Extra Entity Functions
void SSZEggman_Unknown1(void);
void SSZEggman_Unknown2(void);
void SSZEggman_Unknown3(void);
void SSZEggman_Unknown4(void);
void SSZEggman_Unknown5(void);
#endif

#endif //!OBJ_SSZEGGMAN_H
