#ifndef OBJ_LRZFIREBALL_H
#define OBJ_LRZFIREBALL_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectLRZFireball;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityLRZFireball;

// Object Struct
extern ObjectLRZFireball *LRZFireball;

// Standard Entity Events
void LRZFireball_Update();
void LRZFireball_LateUpdate();
void LRZFireball_StaticUpdate();
void LRZFireball_Draw();
void LRZFireball_Create(void* data);
void LRZFireball_StageLoad();
void LRZFireball_EditorDraw();
void LRZFireball_EditorLoad();
void LRZFireball_Serialize();

// Extra Entity Functions


#endif //!OBJ_LRZFIREBALL_H
