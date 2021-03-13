#ifndef OBJ_TETHERBALL_H
#define OBJ_TETHERBALL_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectTetherBall;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityTetherBall;

// Object Struct
extern ObjectTetherBall *TetherBall;

// Standard Entity Events
void TetherBall_Update();
void TetherBall_LateUpdate();
void TetherBall_StaticUpdate();
void TetherBall_Draw();
void TetherBall_Create(void* data);
void TetherBall_StageLoad();
void TetherBall_EditorDraw();
void TetherBall_EditorLoad();
void TetherBall_Serialize();

// Extra Entity Functions


#endif //!OBJ_TETHERBALL_H
