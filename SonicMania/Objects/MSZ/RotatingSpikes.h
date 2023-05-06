#ifndef OBJ_ROTATINGSPIKES_H
#define OBJ_ROTATINGSPIKES_H

#include "Game.h"

typedef enum {
    ROTSPIKES_PRIO_LOW,
    ROTSPIKES_PRIO_HIGH,
} RotatingSpikesPriorities;

// Object Class
struct ObjectRotatingSpikes {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityRotatingSpikes {
    RSDK_ENTITY
    Animator pivotAnimator;
    Animator spikeBallAnimator;
    int32 pivotRadius;
    int32 spikeRadius;
    int32 spikeCount;
    int32 rotSpeed;
    int32 angleOffset;
    int32 priority;
};

// Object Struct
extern ObjectRotatingSpikes *RotatingSpikes;

// Standard Entity Events
void RotatingSpikes_Update(void);
void RotatingSpikes_LateUpdate(void);
void RotatingSpikes_StaticUpdate(void);
void RotatingSpikes_Draw(void);
void RotatingSpikes_Create(void *data);
void RotatingSpikes_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void RotatingSpikes_EditorDraw(void);
void RotatingSpikes_EditorLoad(void);
#endif
void RotatingSpikes_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_ROTATINGSPIKES_H
