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
void Bubbler_Update(void);
void Bubbler_LateUpdate(void);
void Bubbler_StaticUpdate(void);
void Bubbler_Draw(void);
void Bubbler_Create(void* data);
void Bubbler_StageLoad(void);
void Bubbler_EditorDraw(void);
void Bubbler_EditorLoad(void);
void Bubbler_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_BUBBLER_H
