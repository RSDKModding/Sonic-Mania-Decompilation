#ifndef OBJ_PROPELLER_H
#define OBJ_PROPELLER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectPropeller;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityPropeller;

// Object Struct
extern ObjectPropeller *Propeller;

// Standard Entity Events
void Propeller_Update();
void Propeller_LateUpdate();
void Propeller_StaticUpdate();
void Propeller_Draw();
void Propeller_Create(void* data);
void Propeller_StageLoad();
void Propeller_EditorDraw();
void Propeller_EditorLoad();
void Propeller_Serialize();

// Extra Entity Functions


#endif //!OBJ_PROPELLER_H
