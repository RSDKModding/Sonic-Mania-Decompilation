#ifndef OBJ_PSZEGGMAN_H
#define OBJ_PSZEGGMAN_H

#include "../SonicMania.h"

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
void PSZEggman_Update();
void PSZEggman_LateUpdate();
void PSZEggman_StaticUpdate();
void PSZEggman_Draw();
void PSZEggman_Create(void* data);
void PSZEggman_StageLoad();
void PSZEggman_EditorDraw();
void PSZEggman_EditorLoad();
void PSZEggman_Serialize();

// Extra Entity Functions


#endif //!OBJ_PSZEGGMAN_H
