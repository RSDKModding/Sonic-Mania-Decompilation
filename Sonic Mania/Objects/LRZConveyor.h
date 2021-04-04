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
void LRZConveyor_Update(void);
void LRZConveyor_LateUpdate(void);
void LRZConveyor_StaticUpdate(void);
void LRZConveyor_Draw(void);
void LRZConveyor_Create(void* data);
void LRZConveyor_StageLoad(void);
void LRZConveyor_EditorDraw(void);
void LRZConveyor_EditorLoad(void);
void LRZConveyor_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_LRZCONVEYOR_H
