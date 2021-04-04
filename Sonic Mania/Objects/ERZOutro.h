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
void ERZOutro_Update(void);
void ERZOutro_LateUpdate(void);
void ERZOutro_StaticUpdate(void);
void ERZOutro_Draw(void);
void ERZOutro_Create(void* data);
void ERZOutro_StageLoad(void);
void ERZOutro_EditorDraw(void);
void ERZOutro_EditorLoad(void);
void ERZOutro_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_ERZOUTRO_H
