#ifndef OBJ_TOXOMISTER_H
#define OBJ_TOXOMISTER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectToxomister;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityToxomister;

// Object Struct
extern ObjectToxomister *Toxomister;

// Standard Entity Events
void Toxomister_Update();
void Toxomister_LateUpdate();
void Toxomister_StaticUpdate();
void Toxomister_Draw();
void Toxomister_Create(void* data);
void Toxomister_StageLoad();
void Toxomister_EditorDraw();
void Toxomister_EditorLoad();
void Toxomister_Serialize();

// Extra Entity Functions


#endif //!OBJ_TOXOMISTER_H
