#ifndef OBJ_TURBOTURTLE_H
#define OBJ_TURBOTURTLE_H

#include "../SonicMania.h"

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
void TurboTurtle_Update();
void TurboTurtle_LateUpdate();
void TurboTurtle_StaticUpdate();
void TurboTurtle_Draw();
void TurboTurtle_Create(void* data);
void TurboTurtle_StageLoad();
void TurboTurtle_EditorDraw();
void TurboTurtle_EditorLoad();
void TurboTurtle_Serialize();

// Extra Entity Functions


#endif //!OBJ_TURBOTURTLE_H
