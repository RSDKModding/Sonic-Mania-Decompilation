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
void FXExpandRing_Update(void);
void FXExpandRing_LateUpdate(void);
void FXExpandRing_StaticUpdate(void);
void FXExpandRing_Draw(void);
void FXExpandRing_Create(void* data);
void FXExpandRing_StageLoad(void);
void FXExpandRing_EditorDraw(void);
void FXExpandRing_EditorLoad(void);
void FXExpandRing_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_FXEXPANDRING_H
