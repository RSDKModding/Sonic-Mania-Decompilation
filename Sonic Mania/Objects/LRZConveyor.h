#ifndef OBJ_LRZCONVEYOR_H
#define OBJ_LRZCONVEYOR_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectLRZConveyor;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityLRZConveyor;

// Object Struct
extern ObjectLRZConveyor *LRZConveyor;

// Standard Entity Events
void LRZConveyor_Update();
void LRZConveyor_LateUpdate();
void LRZConveyor_StaticUpdate();
void LRZConveyor_Draw();
void LRZConveyor_Create(void* data);
void LRZConveyor_StageLoad();
void LRZConveyor_EditorDraw();
void LRZConveyor_EditorLoad();
void LRZConveyor_Serialize();

// Extra Entity Functions


#endif //!OBJ_LRZCONVEYOR_H
