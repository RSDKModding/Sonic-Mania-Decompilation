#ifndef OBJ_SOL_H
#define OBJ_SOL_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectSol;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntitySol;

// Object Struct
extern ObjectSol *Sol;

// Standard Entity Events
void Sol_Update();
void Sol_LateUpdate();
void Sol_StaticUpdate();
void Sol_Draw();
void Sol_Create(void* data);
void Sol_StageLoad();
void Sol_EditorDraw();
void Sol_EditorLoad();
void Sol_Serialize();

// Extra Entity Functions


#endif //!OBJ_SOL_H
