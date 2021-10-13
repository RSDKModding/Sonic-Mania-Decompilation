#ifndef OBJ_ROTATINGSPIKES_H
#define OBJ_ROTATINGSPIKES_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectRotatingSpikes;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Animator animator1;
    Animator animator2;
    int32 pivotRadius;
    int32 spikeRadius;
    int32 spikeCount;
    int32 rotSpeed;
    int32 angleOffset;
    int32 priority;
} EntityRotatingSpikes;

// Object Struct
extern ObjectRotatingSpikes *RotatingSpikes;

// Standard Entity Events
void RotatingSpikes_Update(void);
void RotatingSpikes_LateUpdate(void);
void RotatingSpikes_StaticUpdate(void);
void RotatingSpikes_Draw(void);
void RotatingSpikes_Create(void* data);
void RotatingSpikes_StageLoad(void);
void RotatingSpikes_EditorDraw(void);
void RotatingSpikes_EditorLoad(void);
void RotatingSpikes_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_ROTATINGSPIKES_H
