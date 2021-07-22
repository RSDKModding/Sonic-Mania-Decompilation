#ifndef OBJ_SHIVERSAW_H
#define OBJ_SHIVERSAW_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectShiversaw;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityShiversaw;

// Object Struct
extern ObjectShiversaw *Shiversaw;

// Standard Entity Events
void Shiversaw_Update(void);
void Shiversaw_LateUpdate(void);
void Shiversaw_StaticUpdate(void);
void Shiversaw_Draw(void);
void Shiversaw_Create(void* data);
void Shiversaw_StageLoad(void);
void Shiversaw_EditorDraw(void);
void Shiversaw_EditorLoad(void);
void Shiversaw_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_SHIVERSAW_H
