#ifndef OBJ_BUBBLER_H
#define OBJ_BUBBLER_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectBubbler;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityBubbler;

// Object Entity
extern ObjectBubbler *Bubbler;

// Standard Entity Events
void Bubbler_Update();
void Bubbler_LateUpdate();
void Bubbler_StaticUpdate();
void Bubbler_Draw();
void Bubbler_Create(void* data);
void Bubbler_StageLoad();
void Bubbler_EditorDraw();
void Bubbler_EditorLoad();
void Bubbler_Serialize();

// Extra Entity Functions


#endif //!OBJ_BUBBLER_H
