#ifndef OBJ_PINATA_H
#define OBJ_PINATA_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPinata;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPinata;

// Object Struct
extern ObjectPinata *Pinata;

// Standard Entity Events
void Pinata_Update();
void Pinata_LateUpdate();
void Pinata_StaticUpdate();
void Pinata_Draw();
void Pinata_Create(void* data);
void Pinata_StageLoad();
void Pinata_EditorDraw();
void Pinata_EditorLoad();
void Pinata_Serialize();

// Extra Entity Functions


#endif //!OBJ_PINATA_H
