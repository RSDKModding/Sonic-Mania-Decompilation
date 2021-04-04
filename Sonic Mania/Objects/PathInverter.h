#ifndef OBJ_PATHINVERTER_H
#define OBJ_PATHINVERTER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPathInverter;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPathInverter;

// Object Struct
extern ObjectPathInverter *PathInverter;

// Standard Entity Events
void PathInverter_Update(void);
void PathInverter_LateUpdate(void);
void PathInverter_StaticUpdate(void);
void PathInverter_Draw(void);
void PathInverter_Create(void* data);
void PathInverter_StageLoad(void);
void PathInverter_EditorDraw(void);
void PathInverter_EditorLoad(void);
void PathInverter_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PATHINVERTER_H
