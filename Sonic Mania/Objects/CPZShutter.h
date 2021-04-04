#ifndef OBJ_CPZSHUTTER_H
#define OBJ_CPZSHUTTER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectCPZShutter;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityCPZShutter;

// Object Struct
extern ObjectCPZShutter *CPZShutter;

// Standard Entity Events
void CPZShutter_Update(void);
void CPZShutter_LateUpdate(void);
void CPZShutter_StaticUpdate(void);
void CPZShutter_Draw(void);
void CPZShutter_Create(void* data);
void CPZShutter_StageLoad(void);
void CPZShutter_EditorDraw(void);
void CPZShutter_EditorLoad(void);
void CPZShutter_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_CPZSHUTTER_H
