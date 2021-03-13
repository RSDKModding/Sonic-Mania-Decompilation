#ifndef OBJ_CHECKERBALL_H
#define OBJ_CHECKERBALL_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectCheckerBall;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityCheckerBall;

// Object Struct
extern ObjectCheckerBall *CheckerBall;

// Standard Entity Events
void CheckerBall_Update();
void CheckerBall_LateUpdate();
void CheckerBall_StaticUpdate();
void CheckerBall_Draw();
void CheckerBall_Create(void* data);
void CheckerBall_StageLoad();
void CheckerBall_EditorDraw();
void CheckerBall_EditorLoad();
void CheckerBall_Serialize();

// Extra Entity Functions


#endif //!OBJ_CHECKERBALL_H
