#ifndef OBJ_PSZEGGMAN_H
#define OBJ_PSZEGGMAN_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPSZEggman;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPSZEggman;

// Object Struct
extern ObjectPSZEggman *PSZEggman;

// Standard Entity Events
void PSZEggman_Update(void);
void PSZEggman_LateUpdate(void);
void PSZEggman_StaticUpdate(void);
void PSZEggman_Draw(void);
void PSZEggman_Create(void* data);
void PSZEggman_StageLoad(void);
void PSZEggman_EditorDraw(void);
void PSZEggman_EditorLoad(void);
void PSZEggman_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PSZEGGMAN_H
