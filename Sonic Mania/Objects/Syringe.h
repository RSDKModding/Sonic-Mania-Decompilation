#ifndef OBJ_SYRINGE_H
#define OBJ_SYRINGE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectSyringe;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntitySyringe;

// Object Struct
extern ObjectSyringe *Syringe;

// Standard Entity Events
void Syringe_Update();
void Syringe_LateUpdate();
void Syringe_StaticUpdate();
void Syringe_Draw();
void Syringe_Create(void* data);
void Syringe_StageLoad();
void Syringe_EditorDraw();
void Syringe_EditorLoad();
void Syringe_Serialize();

// Extra Entity Functions


#endif //!OBJ_SYRINGE_H
