#ifndef OBJ_TITLEEGGMAN_H
#define OBJ_TITLEEGGMAN_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 spriteIndex;
} ObjectTitleEggman;

// Entity Class
typedef struct {
	RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    Vector2 startPos;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
} EntityTitleEggman;

// Object Struct
extern ObjectTitleEggman *TitleEggman;

// Standard Entity Events
void TitleEggman_Update(void);
void TitleEggman_LateUpdate(void);
void TitleEggman_StaticUpdate(void);
void TitleEggman_Draw(void);
void TitleEggman_Create(void* data);
void TitleEggman_StageLoad(void);
void TitleEggman_EditorDraw(void);
void TitleEggman_EditorLoad(void);
void TitleEggman_Serialize(void);

// Extra Entity Functions
void TitleEggman_Unknown1(void);
void TitleEggman_Unknown2(void);
void TitleEggman_Unknown3(void);
#endif

#endif //!OBJ_TITLEEGGMAN_H
