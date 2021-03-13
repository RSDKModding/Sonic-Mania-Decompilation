#ifndef OBJ_ESCAPECAR_H
#define OBJ_ESCAPECAR_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectEscapeCar;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityEscapeCar;

// Object Struct
extern ObjectEscapeCar *EscapeCar;

// Standard Entity Events
void EscapeCar_Update();
void EscapeCar_LateUpdate();
void EscapeCar_StaticUpdate();
void EscapeCar_Draw();
void EscapeCar_Create(void* data);
void EscapeCar_StageLoad();
void EscapeCar_EditorDraw();
void EscapeCar_EditorLoad();
void EscapeCar_Serialize();

// Extra Entity Functions


#endif //!OBJ_ESCAPECAR_H
