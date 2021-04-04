#ifndef OBJ_SPLATS_H
#define OBJ_SPLATS_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectSplats;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntitySplats;

// Object Struct
extern ObjectSplats *Splats;

// Standard Entity Events
void Splats_Update(void);
void Splats_LateUpdate(void);
void Splats_StaticUpdate(void);
void Splats_Draw(void);
void Splats_Create(void* data);
void Splats_StageLoad(void);
void Splats_EditorDraw(void);
void Splats_EditorLoad(void);
void Splats_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_SPLATS_H
