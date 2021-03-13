#ifndef OBJ_BATBOT_H
#define OBJ_BATBOT_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectBatbot;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityBatbot;

// Object Entity
extern ObjectBatbot *Batbot;

// Standard Entity Events
void Batbot_Update();
void Batbot_LateUpdate();
void Batbot_StaticUpdate();
void Batbot_Draw();
void Batbot_Create(void* data);
void Batbot_StageLoad();
void Batbot_EditorDraw();
void Batbot_EditorLoad();
void Batbot_Serialize();

// Extra Entity Functions


#endif //!OBJ_BATBOT_H
