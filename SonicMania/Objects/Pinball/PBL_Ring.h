#ifndef OBJ_PBL_RING_H
#define OBJ_PBL_RING_H

#include "Game.h"

#if MANIA_USE_PLUS
// Object Class
struct ObjectPBL_Ring {
    RSDK_OBJECT
    int32 unused1;
    int32 unused2;
    uint16 aniFrames;
    uint16 sfxRing;
};

// Entity Class
struct EntityPBL_Ring {
    RSDK_ENTITY
    StateMachine(state);
    int32 unused1;
    int32 height;
    int32 unused2;
    int32 worldX;
    int32 worldY;
    Animator animator;
};

// Object Struct
extern ObjectPBL_Ring *PBL_Ring;

// Standard Entity Events
void PBL_Ring_Update(void);
void PBL_Ring_LateUpdate(void);
void PBL_Ring_StaticUpdate(void);
void PBL_Ring_Draw(void);
void PBL_Ring_Create(void *data);
void PBL_Ring_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PBL_Ring_EditorDraw(void);
void PBL_Ring_EditorLoad(void);
#endif
void PBL_Ring_Serialize(void);

// Extra Entity Functions
void PBL_Ring_GiveRing(void);
void PBL_Ring_State_Ring(void);
void PBL_Ring_State_RingSparkle(void);

#endif

#endif //! OBJ_PBL_RING_H
