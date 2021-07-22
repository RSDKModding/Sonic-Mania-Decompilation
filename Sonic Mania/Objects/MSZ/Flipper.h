#ifndef OBJ_FLIPPER_H
#define OBJ_FLIPPER_H

#include "SonicMania.h"

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
void Flipper_Update(void);
void Flipper_LateUpdate(void);
void Flipper_StaticUpdate(void);
void Flipper_Draw(void);
void Flipper_Create(void* data);
void Flipper_StageLoad(void);
void Flipper_EditorDraw(void);
void Flipper_EditorLoad(void);
void Flipper_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_FLIPPER_H
