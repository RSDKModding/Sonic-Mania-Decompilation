#ifndef OBJ_POHBEE_H
#define OBJ_POHBEE_H

#include "../SonicMania.h"

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
void PohBee_Update();
void PohBee_LateUpdate();
void PohBee_StaticUpdate();
void PohBee_Draw();
void PohBee_Create(void* data);
void PohBee_StageLoad();
void PohBee_EditorDraw();
void PohBee_EditorLoad();
void PohBee_Serialize();

// Extra Entity Functions


#endif //!OBJ_POHBEE_H
