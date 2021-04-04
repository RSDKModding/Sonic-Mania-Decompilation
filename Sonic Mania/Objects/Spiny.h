#ifndef OBJ_SPINY_H
#define OBJ_SPINY_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectSpiny;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntitySpiny;

// Object Struct
extern ObjectSpiny *Spiny;

// Standard Entity Events
void Spiny_Update(void);
void Spiny_LateUpdate(void);
void Spiny_StaticUpdate(void);
void Spiny_Draw(void);
void Spiny_Create(void* data);
void Spiny_StageLoad(void);
void Spiny_EditorDraw(void);
void Spiny_EditorLoad(void);
void Spiny_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_SPINY_H
