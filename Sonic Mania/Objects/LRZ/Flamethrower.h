#ifndef OBJ_FLAMETHROWER_H
#define OBJ_FLAMETHROWER_H

#include "SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectFlamethrower;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityFlamethrower;

// Object Struct
extern ObjectFlamethrower *Flamethrower;

// Standard Entity Events
void Flamethrower_Update(void);
void Flamethrower_LateUpdate(void);
void Flamethrower_StaticUpdate(void);
void Flamethrower_Draw(void);
void Flamethrower_Create(void* data);
void Flamethrower_StageLoad(void);
void Flamethrower_EditorDraw(void);
void Flamethrower_EditorLoad(void);
void Flamethrower_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_FLAMETHROWER_H
