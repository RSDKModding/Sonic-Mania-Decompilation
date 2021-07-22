#ifndef OBJ_RISINGLAVA_H
#define OBJ_RISINGLAVA_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectRisingLava;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityRisingLava;

// Object Struct
extern ObjectRisingLava *RisingLava;

// Standard Entity Events
void RisingLava_Update(void);
void RisingLava_LateUpdate(void);
void RisingLava_StaticUpdate(void);
void RisingLava_Draw(void);
void RisingLava_Create(void* data);
void RisingLava_StageLoad(void);
void RisingLava_EditorDraw(void);
void RisingLava_EditorLoad(void);
void RisingLava_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_RISINGLAVA_H
