#ifndef OBJ_BUZZSAW_H
#define OBJ_BUZZSAW_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectBuzzSaw;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityBuzzSaw;

// Object Struct
extern ObjectBuzzSaw *BuzzSaw;

// Standard Entity Events
void BuzzSaw_Update(void);
void BuzzSaw_LateUpdate(void);
void BuzzSaw_StaticUpdate(void);
void BuzzSaw_Draw(void);
void BuzzSaw_Create(void* data);
void BuzzSaw_StageLoad(void);
void BuzzSaw_EditorDraw(void);
void BuzzSaw_EditorLoad(void);
void BuzzSaw_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_BUZZSAW_H
