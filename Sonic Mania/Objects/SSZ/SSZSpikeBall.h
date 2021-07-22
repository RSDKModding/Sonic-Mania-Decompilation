#ifndef OBJ_SSZSPIKEBALL_H
#define OBJ_SSZSPIKEBALL_H

#include "SonicMania.h"

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
void SSZSpikeBall_Update(void);
void SSZSpikeBall_LateUpdate(void);
void SSZSpikeBall_StaticUpdate(void);
void SSZSpikeBall_Draw(void);
void SSZSpikeBall_Create(void* data);
void SSZSpikeBall_StageLoad(void);
void SSZSpikeBall_EditorDraw(void);
void SSZSpikeBall_EditorLoad(void);
void SSZSpikeBall_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_SSZSPIKEBALL_H
