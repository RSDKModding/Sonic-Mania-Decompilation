#ifndef OBJ_DIVEEGGMAN_H
#define OBJ_DIVEEGGMAN_H

#include "SonicMania.h"

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
void DiveEggman_Update(void);
void DiveEggman_LateUpdate(void);
void DiveEggman_StaticUpdate(void);
void DiveEggman_Draw(void);
void DiveEggman_Create(void* data);
void DiveEggman_StageLoad(void);
void DiveEggman_EditorDraw(void);
void DiveEggman_EditorLoad(void);
void DiveEggman_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_DIVEEGGMAN_H
