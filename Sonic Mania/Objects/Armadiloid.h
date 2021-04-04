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
void Armadiloid_Update(void);
void Armadiloid_LateUpdate(void);
void Armadiloid_StaticUpdate(void);
void Armadiloid_Draw(void);
void Armadiloid_Create(void* data);
void Armadiloid_StageLoad(void);
void Armadiloid_EditorDraw(void);
void Armadiloid_EditorLoad(void);
void Armadiloid_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_ARMADILOID_H
