#ifndef OBJ_TMZBARRIER_H
#define OBJ_TMZBARRIER_H

#include "SonicMania.h"

// Object Class
struct ObjectTMZBarrier {
    RSDK_OBJECT
    uint8 flags;
    int32 postID;
};

// Entity Class
struct EntityTMZBarrier {
    RSDK_ENTITY
    uint8 type;
    uint8 warpTag;
    uint8 iD;
    int32 field_5C;
};

// Object Struct
extern ObjectTMZBarrier *TMZBarrier;

// Standard Entity Events
void TMZBarrier_Update(void);
void TMZBarrier_LateUpdate(void);
void TMZBarrier_StaticUpdate(void);
void TMZBarrier_Draw(void);
void TMZBarrier_Create(void* data);
void TMZBarrier_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void TMZBarrier_EditorDraw(void);
void TMZBarrier_EditorLoad(void);
#endif
void TMZBarrier_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_TMZBARRIER_H
