#ifndef OBJ_TUESDAY_H
#define OBJ_TUESDAY_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectTuesday;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityTuesday;

// Object Struct
extern ObjectTuesday *Tuesday;

// Standard Entity Events
void Tuesday_Update(void);
void Tuesday_LateUpdate(void);
void Tuesday_StaticUpdate(void);
void Tuesday_Draw(void);
void Tuesday_Create(void* data);
void Tuesday_StageLoad(void);
void Tuesday_EditorDraw(void);
void Tuesday_EditorLoad(void);
void Tuesday_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_TUESDAY_H
