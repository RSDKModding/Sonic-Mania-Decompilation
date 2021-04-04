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
void EscapeCar_Update(void);
void EscapeCar_LateUpdate(void);
void EscapeCar_StaticUpdate(void);
void EscapeCar_Draw(void);
void EscapeCar_Create(void* data);
void EscapeCar_StageLoad(void);
void EscapeCar_EditorDraw(void);
void EscapeCar_EditorLoad(void);
void EscapeCar_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_ESCAPECAR_H
