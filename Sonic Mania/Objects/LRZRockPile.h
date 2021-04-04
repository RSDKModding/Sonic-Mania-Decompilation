#ifndef OBJ_LRZROCKPILE_H
#define OBJ_LRZROCKPILE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectLRZRockPile;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityLRZRockPile;

// Object Struct
extern ObjectLRZRockPile *LRZRockPile;

// Standard Entity Events
void LRZRockPile_Update(void);
void LRZRockPile_LateUpdate(void);
void LRZRockPile_StaticUpdate(void);
void LRZRockPile_Draw(void);
void LRZRockPile_Create(void* data);
void LRZRockPile_StageLoad(void);
void LRZRockPile_EditorDraw(void);
void LRZRockPile_EditorLoad(void);
void LRZRockPile_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_LRZROCKPILE_H
