#ifndef OBJ_FAN_H
#define OBJ_FAN_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectFan;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityFan;

// Object Struct
extern ObjectFan *Fan;

// Standard Entity Events
void Fan_Update(void);
void Fan_LateUpdate(void);
void Fan_StaticUpdate(void);
void Fan_Draw(void);
void Fan_Create(void* data);
void Fan_StageLoad(void);
void Fan_EditorDraw(void);
void Fan_EditorLoad(void);
void Fan_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_FAN_H
