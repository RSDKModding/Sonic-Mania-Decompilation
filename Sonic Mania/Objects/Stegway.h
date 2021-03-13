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
void Stegway_Update();
void Stegway_LateUpdate();
void Stegway_StaticUpdate();
void Stegway_Draw();
void Stegway_Create(void* data);
void Stegway_StageLoad();
void Stegway_EditorDraw();
void Stegway_EditorLoad();
void Stegway_Serialize();

// Extra Entity Functions


#endif //!OBJ_STEGWAY_H
