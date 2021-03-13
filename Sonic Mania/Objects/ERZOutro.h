#ifndef OBJ_ERZOUTRO_H
#define OBJ_ERZOUTRO_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectERZOutro;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityERZOutro;

// Object Struct
extern ObjectERZOutro *ERZOutro;

// Standard Entity Events
void ERZOutro_Update();
void ERZOutro_LateUpdate();
void ERZOutro_StaticUpdate();
void ERZOutro_Draw();
void ERZOutro_Create(void* data);
void ERZOutro_StageLoad();
void ERZOutro_EditorDraw();
void ERZOutro_EditorLoad();
void ERZOutro_Serialize();

// Extra Entity Functions


#endif //!OBJ_ERZOUTRO_H
