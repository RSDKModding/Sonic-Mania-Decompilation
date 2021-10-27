#ifndef OBJ_BUCKWILDBALL_H
#define OBJ_BUCKWILDBALL_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectBuckwildBall;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityBuckwildBall;

// Object Struct
extern ObjectBuckwildBall *BuckwildBall;

// Standard Entity Events
void BuckwildBall_Update(void);
void BuckwildBall_LateUpdate(void);
void BuckwildBall_StaticUpdate(void);
void BuckwildBall_Draw(void);
void BuckwildBall_Create(void* data);
void BuckwildBall_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void BuckwildBall_EditorDraw(void);
void BuckwildBall_EditorLoad(void);
#endif
void BuckwildBall_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_BUCKWILDBALL_H
