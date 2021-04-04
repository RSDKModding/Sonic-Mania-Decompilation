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
void Ball_Update(void);
void Ball_LateUpdate(void);
void Ball_StaticUpdate(void);
void Ball_Draw(void);
void Ball_Create(void* data);
void Ball_StageLoad(void);
void Ball_EditorDraw(void);
void Ball_EditorLoad(void);
void Ball_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_BALL_H
