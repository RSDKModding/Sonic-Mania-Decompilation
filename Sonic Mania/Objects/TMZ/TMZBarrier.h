#ifndef OBJ_TMZBARRIER_H
#define OBJ_TMZBARRIER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    byte flags;
    int postID;
} ObjectTMZBarrier;

// Entity Class
typedef struct {
    RSDK_ENTITY
    byte type;
    byte warpTag;
    byte iD;
    int field_5C;
} EntityTMZBarrier;

// Object Struct
extern ObjectTMZBarrier *TMZBarrier;

// Standard Entity Events
void TMZBarrier_Update(void);
void TMZBarrier_LateUpdate(void);
void TMZBarrier_StaticUpdate(void);
void TMZBarrier_Draw(void);
void TMZBarrier_Create(void* data);
void TMZBarrier_StageLoad(void);
void TMZBarrier_EditorDraw(void);
void TMZBarrier_EditorLoad(void);
void TMZBarrier_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_TMZBARRIER_H
