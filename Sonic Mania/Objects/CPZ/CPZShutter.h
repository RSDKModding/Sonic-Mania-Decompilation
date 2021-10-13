#ifndef OBJ_CPZSHUTTER_H
#define OBJ_CPZSHUTTER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectCPZShutter;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 field_60;
    int32 field_64;
    Animator animator;
} EntityCPZShutter;

// Object Struct
extern ObjectCPZShutter *CPZShutter;

// Standard Entity Events
void CPZShutter_Update(void);
void CPZShutter_LateUpdate(void);
void CPZShutter_StaticUpdate(void);
void CPZShutter_Draw(void);
void CPZShutter_Create(void* data);
void CPZShutter_StageLoad(void);
void CPZShutter_EditorDraw(void);
void CPZShutter_EditorLoad(void);
void CPZShutter_Serialize(void);

// Extra Entity Functions
void CPZShutter_State_Open(void);
void CPZShutter_State_Close(void);

#endif //!OBJ_CPZSHUTTER_H
