#ifndef OBJ_ROTATINGSTAIR_H
#define OBJ_ROTATINGSTAIR_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectRotatingStair;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityRotatingStair;

// Object Struct
extern ObjectRotatingStair *RotatingStair;

// Standard Entity Events
void RotatingStair_Update();
void RotatingStair_LateUpdate();
void RotatingStair_StaticUpdate();
void RotatingStair_Draw();
void RotatingStair_Create(void* data);
void RotatingStair_StageLoad();
void RotatingStair_EditorDraw();
void RotatingStair_EditorLoad();
void RotatingStair_Serialize();

// Extra Entity Functions


#endif //!OBJ_ROTATINGSTAIR_H
