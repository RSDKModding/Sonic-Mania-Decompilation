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
void CPZShutter_Update();
void CPZShutter_LateUpdate();
void CPZShutter_StaticUpdate();
void CPZShutter_Draw();
void CPZShutter_Create(void* data);
void CPZShutter_StageLoad();
void CPZShutter_EditorDraw();
void CPZShutter_EditorLoad();
void CPZShutter_Serialize();

// Extra Entity Functions


#endif //!OBJ_CPZSHUTTER_H
