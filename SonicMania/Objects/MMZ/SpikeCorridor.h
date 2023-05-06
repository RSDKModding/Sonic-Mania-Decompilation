#ifndef OBJ_SPIKECORRIDOR_H
#define OBJ_SPIKECORRIDOR_H

#include "Game.h"

// Object Class
struct ObjectSpikeCorridor {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxDrop;
    uint16 sfxImpact;
    uint16 sfxIndicator;
};

// Entity Class
struct EntitySpikeCorridor {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    Vector2 startPos;
    int8 colWidth;
    int8 rowHeight;
    int32 yOffset;
    int32 timer;
    int8 rowID;
    int8 lastSize;
    int8 storedRowID;
    int8 size;
    int32 fallOffset;
    Hitbox hitbox;
    Hitbox hitboxes[4];
    EntitySpikeCorridor *parent;
};

// Object Struct
extern ObjectSpikeCorridor *SpikeCorridor;

// Standard Entity Events
void SpikeCorridor_Update(void);
void SpikeCorridor_LateUpdate(void);
void SpikeCorridor_StaticUpdate(void);
void SpikeCorridor_Draw(void);
void SpikeCorridor_Create(void *data);
void SpikeCorridor_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void SpikeCorridor_EditorDraw(void);
void SpikeCorridor_EditorLoad(void);
#endif
void SpikeCorridor_Serialize(void);

// Extra Entity Functions
void SpikeCorridor_SetupHitboxes(void);
void SpikeCorridor_HandleDrawing(Animator *animator, int32 offsetY, int8 size, bool32 useWarnAnim);
void SpikeCorridor_SetupNextSpikeRow(void);
void SpikeCorridor_CheckPlayerCollisions(void);

void SpikeCorridor_StateDropper_Setup(void);
void SpikeCorridor_StateDropper_CheckForPlayer(void);
void SpikeCorridor_StateDropper_DropWarn(void);
void SpikeCorridor_StateDropper_SpawnSpikes(void);
void SpikeCorridor_StateDropper_DropWait(void);

void SpikeCorridor_StateSpikes_Setup(void);
void SpikeCorridor_StateSpikes_Fall(void);
void SpikeCorridor_StateSpikes_Land(void);

void SpikeCorridor_Draw_DropWarn(void);
void SpikeCorridor_Draw_Spikes(void);

#endif //! OBJ_SPIKECORRIDOR_H
