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
void HeavyMystic_Update();
void HeavyMystic_LateUpdate();
void HeavyMystic_StaticUpdate();
void HeavyMystic_Draw();
void HeavyMystic_Create(void* data);
void HeavyMystic_StageLoad();
void HeavyMystic_EditorDraw();
void HeavyMystic_EditorLoad();
void HeavyMystic_Serialize();

// Extra Entity Functions


#endif //!OBJ_HEAVYMYSTIC_H
