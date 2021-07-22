#ifndef OBJ_POHBEE_H
#define OBJ_POHBEE_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPohBee;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPohBee;

// Object Struct
extern ObjectPohBee *PohBee;

// Standard Entity Events
void PohBee_Update(void);
void PohBee_LateUpdate(void);
void PohBee_StaticUpdate(void);
void PohBee_Draw(void);
void PohBee_Create(void* data);
void PohBee_StageLoad(void);
void PohBee_EditorDraw(void);
void PohBee_EditorLoad(void);
void PohBee_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_POHBEE_H
