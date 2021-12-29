#ifndef OBJ_TMZALERT_H
#define OBJ_TMZALERT_H

#include "SonicMania.h"

// Object Class
struct ObjectTMZAlert {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityTMZAlert {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    Animator animator;
    Animator animator2;
};

// Object Struct
extern ObjectTMZAlert *TMZAlert;

// Standard Entity Events
void TMZAlert_Update(void);
void TMZAlert_LateUpdate(void);
void TMZAlert_StaticUpdate(void);
void TMZAlert_Draw(void);
void TMZAlert_Create(void* data);
void TMZAlert_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void TMZAlert_EditorDraw(void);
void TMZAlert_EditorLoad(void);
#endif
void TMZAlert_Serialize(void);

// Extra Entity Functions
void TMZAlert_Unknown1(void);
void TMZAlert_Unknown2(void);
void TMZAlert_Unknown3(void);


#endif //!OBJ_TMZALERT_H
