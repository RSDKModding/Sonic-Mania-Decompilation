#ifndef OBJ_LARGEGEAR_H
#define OBJ_LARGEGEAR_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectLargeGear;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityLargeGear;

// Object Struct
extern ObjectLargeGear *LargeGear;

// Standard Entity Events
void LargeGear_Update(void);
void LargeGear_LateUpdate(void);
void LargeGear_StaticUpdate(void);
void LargeGear_Draw(void);
void LargeGear_Create(void* data);
void LargeGear_StageLoad(void);
void LargeGear_EditorDraw(void);
void LargeGear_EditorLoad(void);
void LargeGear_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_LARGEGEAR_H
