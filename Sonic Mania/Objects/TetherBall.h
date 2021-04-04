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
void TetherBall_Update(void);
void TetherBall_LateUpdate(void);
void TetherBall_StaticUpdate(void);
void TetherBall_Draw(void);
void TetherBall_Create(void* data);
void TetherBall_StageLoad(void);
void TetherBall_EditorDraw(void);
void TetherBall_EditorLoad(void);
void TetherBall_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_TETHERBALL_H
