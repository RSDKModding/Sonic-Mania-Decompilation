#ifndef OBJ_RPLANESHIFTER_H
#define OBJ_RPLANESHIFTER_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectRPlaneShifter;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityRPlaneShifter;

// Object Struct
extern ObjectRPlaneShifter *RPlaneShifter;

// Standard Entity Events
void RPlaneShifter_Update(void);
void RPlaneShifter_LateUpdate(void);
void RPlaneShifter_StaticUpdate(void);
void RPlaneShifter_Draw(void);
void RPlaneShifter_Create(void* data);
void RPlaneShifter_StageLoad(void);
void RPlaneShifter_EditorDraw(void);
void RPlaneShifter_EditorLoad(void);
void RPlaneShifter_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_RPLANESHIFTER_H
