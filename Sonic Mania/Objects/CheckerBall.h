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
void CheckerBall_Update(void);
void CheckerBall_LateUpdate(void);
void CheckerBall_StaticUpdate(void);
void CheckerBall_Draw(void);
void CheckerBall_Create(void* data);
void CheckerBall_StageLoad(void);
void CheckerBall_EditorDraw(void);
void CheckerBall_EditorLoad(void);
void CheckerBall_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_CHECKERBALL_H
