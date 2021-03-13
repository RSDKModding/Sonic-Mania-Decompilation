#ifndef OBJ_DIVEEGGMAN_H
#define OBJ_DIVEEGGMAN_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectDiveEggman;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityDiveEggman;

// Object Struct
extern ObjectDiveEggman *DiveEggman;

// Standard Entity Events
void DiveEggman_Update();
void DiveEggman_LateUpdate();
void DiveEggman_StaticUpdate();
void DiveEggman_Draw();
void DiveEggman_Create(void* data);
void DiveEggman_StageLoad();
void DiveEggman_EditorDraw();
void DiveEggman_EditorLoad();
void DiveEggman_Serialize();

// Extra Entity Functions


#endif //!OBJ_DIVEEGGMAN_H
