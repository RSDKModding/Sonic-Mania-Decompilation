#ifndef OBJ_STEGWAY_H
#define OBJ_STEGWAY_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectStegway;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityStegway;

// Object Entity
extern ObjectStegway *Stegway;

// Standard Entity Events
void Stegway_Update(void);
void Stegway_LateUpdate(void);
void Stegway_StaticUpdate(void);
void Stegway_Draw(void);
void Stegway_Create(void* data);
void Stegway_StageLoad(void);
void Stegway_EditorDraw(void);
void Stegway_EditorLoad(void);
void Stegway_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_STEGWAY_H
