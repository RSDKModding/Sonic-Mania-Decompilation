#ifndef OBJ_SSZSPIKEBALL_H
#define OBJ_SSZSPIKEBALL_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectSSZSpikeBall;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntitySSZSpikeBall;

// Object Struct
extern ObjectSSZSpikeBall *SSZSpikeBall;

// Standard Entity Events
void SSZSpikeBall_Update();
void SSZSpikeBall_LateUpdate();
void SSZSpikeBall_StaticUpdate();
void SSZSpikeBall_Draw();
void SSZSpikeBall_Create(void* data);
void SSZSpikeBall_StageLoad();
void SSZSpikeBall_EditorDraw();
void SSZSpikeBall_EditorLoad();
void SSZSpikeBall_Serialize();

// Extra Entity Functions


#endif //!OBJ_SSZSPIKEBALL_H
