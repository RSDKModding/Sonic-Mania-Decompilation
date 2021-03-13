#ifndef OBJ_SPINY_H
#define OBJ_SPINY_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectSpiny;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntitySpiny;

// Object Struct
extern ObjectSpiny *Spiny;

// Standard Entity Events
void Spiny_Update();
void Spiny_LateUpdate();
void Spiny_StaticUpdate();
void Spiny_Draw();
void Spiny_Create(void* data);
void Spiny_StageLoad();
void Spiny_EditorDraw();
void Spiny_EditorLoad();
void Spiny_Serialize();

// Extra Entity Functions


#endif //!OBJ_SPINY_H
