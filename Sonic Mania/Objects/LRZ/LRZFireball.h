#ifndef OBJ_LRZFIREBALL_H
#define OBJ_LRZFIREBALL_H

#include "SonicMania.h"

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
void LRZFireball_Update(void);
void LRZFireball_LateUpdate(void);
void LRZFireball_StaticUpdate(void);
void LRZFireball_Draw(void);
void LRZFireball_Create(void* data);
void LRZFireball_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void LRZFireball_EditorDraw(void);
void LRZFireball_EditorLoad(void);
#endif
void LRZFireball_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_LRZFIREBALL_H
