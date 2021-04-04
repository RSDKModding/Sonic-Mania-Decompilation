#ifndef OBJ_TURBOSPIKER_H
#define OBJ_TURBOSPIKER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectTurboSpiker;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityTurboSpiker;

// Object Struct
extern ObjectTurboSpiker *TurboSpiker;

// Standard Entity Events
void TurboSpiker_Update(void);
void TurboSpiker_LateUpdate(void);
void TurboSpiker_StaticUpdate(void);
void TurboSpiker_Draw(void);
void TurboSpiker_Create(void* data);
void TurboSpiker_StageLoad(void);
void TurboSpiker_EditorDraw(void);
void TurboSpiker_EditorLoad(void);
void TurboSpiker_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_TURBOSPIKER_H
