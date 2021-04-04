#ifndef OBJ_HCZSPIKEBALL_H
#define OBJ_HCZSPIKEBALL_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectHCZSpikeBall;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityHCZSpikeBall;

// Object Struct
extern ObjectHCZSpikeBall *HCZSpikeBall;

// Standard Entity Events
void HCZSpikeBall_Update(void);
void HCZSpikeBall_LateUpdate(void);
void HCZSpikeBall_StaticUpdate(void);
void HCZSpikeBall_Draw(void);
void HCZSpikeBall_Create(void* data);
void HCZSpikeBall_StageLoad(void);
void HCZSpikeBall_EditorDraw(void);
void HCZSpikeBall_EditorLoad(void);
void HCZSpikeBall_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_HCZSPIKEBALL_H
