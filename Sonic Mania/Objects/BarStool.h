#ifndef OBJ_BARSTOOL_H
#define OBJ_BARSTOOL_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectBarStool;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityBarStool;

// Object Struct
extern ObjectBarStool *BarStool;

// Standard Entity Events
void BarStool_Update(void);
void BarStool_LateUpdate(void);
void BarStool_StaticUpdate(void);
void BarStool_Draw(void);
void BarStool_Create(void* data);
void BarStool_StageLoad(void);
void BarStool_EditorDraw(void);
void BarStool_EditorLoad(void);
void BarStool_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_BARSTOOL_H
