#ifndef OBJ_TURNTABLE_H
#define OBJ_TURNTABLE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectTurntable;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityTurntable;

// Object Struct
extern ObjectTurntable *Turntable;

// Standard Entity Events
void Turntable_Update(void);
void Turntable_LateUpdate(void);
void Turntable_StaticUpdate(void);
void Turntable_Draw(void);
void Turntable_Create(void* data);
void Turntable_StageLoad(void);
void Turntable_EditorDraw(void);
void Turntable_EditorLoad(void);
void Turntable_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_TURNTABLE_H
