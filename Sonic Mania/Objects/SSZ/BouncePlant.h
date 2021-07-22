#ifndef OBJ_BOUNCEPLANT_H
#define OBJ_BOUNCEPLANT_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectBouncePlant;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityBouncePlant;

// Object Struct
extern ObjectBouncePlant *BouncePlant;

// Standard Entity Events
void BouncePlant_Update(void);
void BouncePlant_LateUpdate(void);
void BouncePlant_StaticUpdate(void);
void BouncePlant_Draw(void);
void BouncePlant_Create(void* data);
void BouncePlant_StageLoad(void);
void BouncePlant_EditorDraw(void);
void BouncePlant_EditorLoad(void);
void BouncePlant_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_BOUNCEPLANT_H
