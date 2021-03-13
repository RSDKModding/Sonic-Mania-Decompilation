#ifndef OBJ_SP500_H
#define OBJ_SP500_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectSP500;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntitySP500;

// Object Struct
extern ObjectSP500 *SP500;

// Standard Entity Events
void SP500_Update();
void SP500_LateUpdate();
void SP500_StaticUpdate();
void SP500_Draw();
void SP500_Create(void* data);
void SP500_StageLoad();
void SP500_EditorDraw();
void SP500_EditorLoad();
void SP500_Serialize();

// Extra Entity Functions


#endif //!OBJ_SP500_H
