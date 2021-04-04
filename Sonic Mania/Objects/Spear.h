#ifndef OBJ_SPEAR_H
#define OBJ_SPEAR_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectSpear;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntitySpear;

// Object Struct
extern ObjectSpear *Spear;

// Standard Entity Events
void Spear_Update(void);
void Spear_LateUpdate(void);
void Spear_StaticUpdate(void);
void Spear_Draw(void);
void Spear_Create(void* data);
void Spear_StageLoad(void);
void Spear_EditorDraw(void);
void Spear_EditorLoad(void);
void Spear_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_SPEAR_H
