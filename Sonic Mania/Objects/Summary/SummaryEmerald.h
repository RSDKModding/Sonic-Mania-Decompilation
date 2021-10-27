#ifndef OBJ_SUMMARYEMERALD_H
#define OBJ_SUMMARYEMERALD_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 spriteIndex;
} ObjectSummaryEmerald;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int32 emeraldID;
    Animator animator;
} EntitySummaryEmerald;

// Object Struct
extern ObjectSummaryEmerald *SummaryEmerald;

// Standard Entity Events
void SummaryEmerald_Update(void);
void SummaryEmerald_LateUpdate(void);
void SummaryEmerald_StaticUpdate(void);
void SummaryEmerald_Draw(void);
void SummaryEmerald_Create(void* data);
void SummaryEmerald_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void SummaryEmerald_EditorDraw(void);
void SummaryEmerald_EditorLoad(void);
#endif
void SummaryEmerald_Serialize(void);

// Extra Entity Functions
#endif

#endif //!OBJ_SUMMARYEMERALD_H
