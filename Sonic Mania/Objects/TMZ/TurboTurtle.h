#ifndef OBJ_TURBOTURTLE_H
#define OBJ_TURBOTURTLE_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectTurboTurtle;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityTurboTurtle;

// Object Struct
extern ObjectTurboTurtle *TurboTurtle;

// Standard Entity Events
void TurboTurtle_Update(void);
void TurboTurtle_LateUpdate(void);
void TurboTurtle_StaticUpdate(void);
void TurboTurtle_Draw(void);
void TurboTurtle_Create(void* data);
void TurboTurtle_StageLoad(void);
void TurboTurtle_EditorDraw(void);
void TurboTurtle_EditorLoad(void);
void TurboTurtle_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_TURBOTURTLE_H
