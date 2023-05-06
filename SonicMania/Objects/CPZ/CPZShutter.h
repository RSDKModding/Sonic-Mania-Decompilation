#ifndef OBJ_CPZSHUTTER_H
#define OBJ_CPZSHUTTER_H

#include "Game.h"

// Object Class
struct ObjectCPZShutter {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityCPZShutter {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 unused1;
    int32 unused2;
    Animator animator;
};

// Object Struct
extern ObjectCPZShutter *CPZShutter;

// Standard Entity Events
void CPZShutter_Update(void);
void CPZShutter_LateUpdate(void);
void CPZShutter_StaticUpdate(void);
void CPZShutter_Draw(void);
void CPZShutter_Create(void *data);
void CPZShutter_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void CPZShutter_EditorDraw(void);
void CPZShutter_EditorLoad(void);
#endif
void CPZShutter_Serialize(void);

// Extra Entity Functions
void CPZShutter_State_Open(void);
void CPZShutter_State_Close(void);

#endif //! OBJ_CPZSHUTTER_H
