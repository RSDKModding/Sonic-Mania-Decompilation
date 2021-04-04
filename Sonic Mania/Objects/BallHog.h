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
void BallHog_Update(void);
void BallHog_LateUpdate(void);
void BallHog_StaticUpdate(void);
void BallHog_Draw(void);
void BallHog_Create(void* data);
void BallHog_StageLoad(void);
void BallHog_EditorDraw(void);
void BallHog_EditorLoad(void);
void BallHog_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_BALLHOG_H
