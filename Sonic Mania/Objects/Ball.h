#ifndef OBJ_BALL_H
#define OBJ_BALL_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectBall;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityBall;

// Object Entity
extern ObjectBall *Ball;

// Standard Entity Events
void Ball_Update();
void Ball_LateUpdate();
void Ball_StaticUpdate();
void Ball_Draw();
void Ball_Create(void* data);
void Ball_StageLoad();
void Ball_EditorDraw();
void Ball_EditorLoad();
void Ball_Serialize();

// Extra Entity Functions


#endif //!OBJ_BALL_H
