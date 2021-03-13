#ifndef OBJ_BUZZSAW_H
#define OBJ_BUZZSAW_H

#include "../SonicMania.h"

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
void BuzzSaw_Update();
void BuzzSaw_LateUpdate();
void BuzzSaw_StaticUpdate();
void BuzzSaw_Draw();
void BuzzSaw_Create(void* data);
void BuzzSaw_StageLoad();
void BuzzSaw_EditorDraw();
void BuzzSaw_EditorLoad();
void BuzzSaw_Serialize();

// Extra Entity Functions


#endif //!OBJ_BUZZSAW_H
