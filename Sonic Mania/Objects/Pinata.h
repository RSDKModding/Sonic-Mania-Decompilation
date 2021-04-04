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
void Pinata_Update(void);
void Pinata_LateUpdate(void);
void Pinata_StaticUpdate(void);
void Pinata_Draw(void);
void Pinata_Create(void* data);
void Pinata_StageLoad(void);
void Pinata_EditorDraw(void);
void Pinata_EditorLoad(void);
void Pinata_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_PINATA_H
