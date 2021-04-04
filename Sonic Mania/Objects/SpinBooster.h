#ifndef OBJ_SPINBOOSTER_H
#define OBJ_SPINBOOSTER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectSpinBooster;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntitySpinBooster;

// Object Struct
extern ObjectSpinBooster *SpinBooster;

// Standard Entity Events
void SpinBooster_Update(void);
void SpinBooster_LateUpdate(void);
void SpinBooster_StaticUpdate(void);
void SpinBooster_Draw(void);
void SpinBooster_Create(void* data);
void SpinBooster_StageLoad(void);
void SpinBooster_EditorDraw(void);
void SpinBooster_EditorLoad(void);
void SpinBooster_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_SPINBOOSTER_H
