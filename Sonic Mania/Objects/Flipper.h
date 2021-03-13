#ifndef OBJ_FLIPPER_H
#define OBJ_FLIPPER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectFlipper;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityFlipper;

// Object Struct
extern ObjectFlipper *Flipper;

// Standard Entity Events
void Flipper_Update();
void Flipper_LateUpdate();
void Flipper_StaticUpdate();
void Flipper_Draw();
void Flipper_Create(void* data);
void Flipper_StageLoad();
void Flipper_EditorDraw();
void Flipper_EditorLoad();
void Flipper_Serialize();

// Extra Entity Functions


#endif //!OBJ_FLIPPER_H
