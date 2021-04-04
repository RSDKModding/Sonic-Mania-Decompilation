#ifndef OBJ_ROTATINGSPIKES_H
#define OBJ_ROTATINGSPIKES_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectRotatingSpikes;

// Entity Class
typedef struct {
	RSDK_ENTITY
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
