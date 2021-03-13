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
void Spear_Update();
void Spear_LateUpdate();
void Spear_StaticUpdate();
void Spear_Draw();
void Spear_Create(void* data);
void Spear_StageLoad();
void Spear_EditorDraw();
void Spear_EditorLoad();
void Spear_Serialize();

// Extra Entity Functions


#endif //!OBJ_SPEAR_H
