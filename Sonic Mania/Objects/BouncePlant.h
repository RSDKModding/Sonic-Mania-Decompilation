#ifndef OBJ_BOUNCEPLANT_H
#define OBJ_BOUNCEPLANT_H

#include "../SonicMania.h"

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
void BouncePlant_Update();
void BouncePlant_LateUpdate();
void BouncePlant_StaticUpdate();
void BouncePlant_Draw();
void BouncePlant_Create(void* data);
void BouncePlant_StageLoad();
void BouncePlant_EditorDraw();
void BouncePlant_EditorLoad();
void BouncePlant_Serialize();

// Extra Entity Functions


#endif //!OBJ_BOUNCEPLANT_H
