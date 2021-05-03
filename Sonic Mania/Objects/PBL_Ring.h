#ifndef OBJ_PBL_RING_H
#define OBJ_PBL_RING_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    int field_4;
    int field_8;
    ushort aniFrames;
    ushort sfxRing;
} ObjectPBL_Ring;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int field_5C;
    int height;
    int field_64;
    int worldX;
    int worldY;
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
void PBL_Ring_EditorDraw(void);
void PBL_Ring_EditorLoad(void);
void PBL_Ring_Serialize(void);

// Extra Entity Functions
void PBL_Ring_GiveRing(void);
void PBL_Ring_State_Ring(void);
void PBL_Ring_State_RingSparkle(void);

#endif

#endif //!OBJ_PBL_RING_H
