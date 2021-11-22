#ifndef OBJ_PBL_RING_H
#define OBJ_PBL_RING_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    int32 unused1;
    int32 unused2;
    uint16 aniFrames;
    uint16 sfxRing;
} ObjectPBL_Ring;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 unused1;
    int32 height;
    int32 unused2;
    int32 worldX;
    int32 worldY;
    Animator animator;
} EntityPBL_Ring;

// Object Struct
extern ObjectPBL_Ring *PBL_Ring;

// Standard Entity Events
void PBL_Ring_Update(void);
void PBL_Ring_LateUpdate(void);
void PBL_Ring_StaticUpdate(void);
void PBL_Ring_Draw(void);
void PBL_Ring_Create(void* data);
void PBL_Ring_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void PBL_Ring_EditorDraw(void);
void PBL_Ring_EditorLoad(void);
#endif
void PBL_Ring_Serialize(void);

// Extra Entity Functions
void PBL_Ring_GiveRing(void);
void PBL_Ring_State_Ring(void);
void PBL_Ring_State_RingSparkle(void);

#endif

#endif //!OBJ_PBL_RING_H
