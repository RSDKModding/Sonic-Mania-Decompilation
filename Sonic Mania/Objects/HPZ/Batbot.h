#ifndef OBJ_BATBOT_H
#define OBJ_BATBOT_H

#include "SonicMania.h"

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
void Batbot_Update(void);
void Batbot_LateUpdate(void);
void Batbot_StaticUpdate(void);
void Batbot_Draw(void);
void Batbot_Create(void* data);
void Batbot_StageLoad(void);
void Batbot_EditorDraw(void);
void Batbot_EditorLoad(void);
void Batbot_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_BATBOT_H
