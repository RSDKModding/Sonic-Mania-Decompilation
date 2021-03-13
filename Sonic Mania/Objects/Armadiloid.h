#ifndef OBJ_ARMADILOID_H
#define OBJ_ARMADILOID_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectArmadiloid;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityArmadiloid;

// Object Struct
extern ObjectArmadiloid *Armadiloid;

// Standard Entity Events
void Armadiloid_Update();
void Armadiloid_LateUpdate();
void Armadiloid_StaticUpdate();
void Armadiloid_Draw();
void Armadiloid_Create(void* data);
void Armadiloid_StageLoad();
void Armadiloid_EditorDraw();
void Armadiloid_EditorLoad();
void Armadiloid_Serialize();

// Extra Entity Functions


#endif //!OBJ_ARMADILOID_H
