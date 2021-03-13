#ifndef OBJ_BALLHOG_H
#define OBJ_BALLHOG_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectBallHog;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityBallHog;

// Object Struct
extern ObjectBallHog *BallHog;

// Standard Entity Events
void BallHog_Update();
void BallHog_LateUpdate();
void BallHog_StaticUpdate();
void BallHog_Draw();
void BallHog_Create(void* data);
void BallHog_StageLoad();
void BallHog_EditorDraw();
void BallHog_EditorLoad();
void BallHog_Serialize();

// Extra Entity Functions


#endif //!OBJ_BALLHOG_H
