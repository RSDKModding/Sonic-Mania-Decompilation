#ifndef OBJ_FXEXPANDRING_H
#define OBJ_FXEXPANDRING_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectFXExpandRing;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityFXExpandRing;

// Object Struct
extern ObjectFXExpandRing *FXExpandRing;

// Standard Entity Events
void FXExpandRing_Update();
void FXExpandRing_LateUpdate();
void FXExpandRing_StaticUpdate();
void FXExpandRing_Draw();
void FXExpandRing_Create(void* data);
void FXExpandRing_StageLoad();
void FXExpandRing_EditorDraw();
void FXExpandRing_EditorLoad();
void FXExpandRing_Serialize();

// Extra Entity Functions


#endif //!OBJ_FXEXPANDRING_H
