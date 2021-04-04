#ifndef OBJ_HEAVYMYSTIC_H
#define OBJ_HEAVYMYSTIC_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectHeavyMystic;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityHeavyMystic;

// Object Struct
extern ObjectHeavyMystic *HeavyMystic;

// Standard Entity Events
void HeavyMystic_Update(void);
void HeavyMystic_LateUpdate(void);
void HeavyMystic_StaticUpdate(void);
void HeavyMystic_Draw(void);
void HeavyMystic_Create(void* data);
void HeavyMystic_StageLoad(void);
void HeavyMystic_EditorDraw(void);
void HeavyMystic_EditorLoad(void);
void HeavyMystic_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_HEAVYMYSTIC_H
