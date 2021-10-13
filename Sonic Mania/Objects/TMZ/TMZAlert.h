#ifndef OBJ_TMZALERT_H
#define OBJ_TMZALERT_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectTMZAlert;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int timer;
    Animator animator;
    Animator animator2;
} EntityTMZAlert;

// Object Struct
extern ObjectTMZAlert *TMZAlert;

// Standard Entity Events
void TMZAlert_Update(void);
void TMZAlert_LateUpdate(void);
void TMZAlert_StaticUpdate(void);
void TMZAlert_Draw(void);
void TMZAlert_Create(void* data);
void TMZAlert_StageLoad(void);
void TMZAlert_EditorDraw(void);
void TMZAlert_EditorLoad(void);
void TMZAlert_Serialize(void);

// Extra Entity Functions
void TMZAlert_Unknown1(void);
void TMZAlert_Unknown2(void);
void TMZAlert_Unknown3(void);


#endif //!OBJ_TMZALERT_H
