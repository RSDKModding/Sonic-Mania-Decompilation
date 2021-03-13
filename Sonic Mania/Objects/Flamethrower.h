#ifndef OBJ_FLAMETHROWER_H
#define OBJ_FLAMETHROWER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectFlamethrower;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityFlamethrower;

// Object Struct
extern ObjectFlamethrower *Flamethrower;

// Standard Entity Events
void Flamethrower_Update();
void Flamethrower_LateUpdate();
void Flamethrower_StaticUpdate();
void Flamethrower_Draw();
void Flamethrower_Create(void* data);
void Flamethrower_StageLoad();
void Flamethrower_EditorDraw();
void Flamethrower_EditorLoad();
void Flamethrower_Serialize();

// Extra Entity Functions


#endif //!OBJ_FLAMETHROWER_H
