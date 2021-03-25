#ifndef OBJ_SPIKELOG_H
#define OBJ_SPIKELOG_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    bool32 hasAchievement;
    byte timer;
    ushort spriteIndex;
    Hitbox hitbox;
    Hitbox burnHitbox;
} ObjectSpikeLog;

// Entity Class
typedef struct {
    RSDK_ENTITY
    void (*state)();
    byte frame;
    AnimationData data;
} EntitySpikeLog;

// Object Struct
extern ObjectSpikeLog *SpikeLog;

// Standard Entity Events
void SpikeLog_Update();
void SpikeLog_LateUpdate();
void SpikeLog_StaticUpdate();
void SpikeLog_Draw();
void SpikeLog_Create(void* data);
void SpikeLog_StageLoad();
void SpikeLog_EditorDraw();
void SpikeLog_EditorLoad();
void SpikeLog_Serialize();

// Extra Entity Functions
void SpikeLog_State_Main();
void SpikeLog_State_Burn();


#endif //!OBJ_SPIKELOG_H
