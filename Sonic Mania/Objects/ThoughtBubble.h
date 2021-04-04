#ifndef OBJ_THOUGHTBUBBLE_H
#define OBJ_THOUGHTBUBBLE_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectThoughtBubble;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityThoughtBubble;

// Object Struct
extern ObjectThoughtBubble *ThoughtBubble;

// Standard Entity Events
void ThoughtBubble_Update(void);
void ThoughtBubble_LateUpdate(void);
void ThoughtBubble_StaticUpdate(void);
void ThoughtBubble_Draw(void);
void ThoughtBubble_Create(void* data);
void ThoughtBubble_StageLoad(void);
void ThoughtBubble_EditorDraw(void);
void ThoughtBubble_EditorLoad(void);
void ThoughtBubble_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_THOUGHTBUBBLE_H
