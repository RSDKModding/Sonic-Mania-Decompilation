#ifndef OBJ_SPIKELOG_H
#define OBJ_SPIKELOG_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    bool32 hasAchievement;
    uint8 timer;
    uint16 aniFrames;
    Hitbox hitbox;
    Hitbox burnHitbox;
} ObjectSpikeLog;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    uint8 frame;
    Animator animator;
} EntitySpikeLog;

// Object Struct
extern ObjectSpikeLog *SpikeLog;

// Standard Entity Events
void SpikeLog_Update(void);
void SpikeLog_LateUpdate(void);
void SpikeLog_StaticUpdate(void);
void SpikeLog_Draw(void);
void SpikeLog_Create(void* data);
void SpikeLog_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void SpikeLog_EditorDraw(void);
void SpikeLog_EditorLoad(void);
#endif
void SpikeLog_Serialize(void);

// Extra Entity Functions
void SpikeLog_State_Main(void);
void SpikeLog_State_Burn(void);


#endif //!OBJ_SPIKELOG_H
