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
void ThoughtBubble_Update();
void ThoughtBubble_LateUpdate();
void ThoughtBubble_StaticUpdate();
void ThoughtBubble_Draw();
void ThoughtBubble_Create(void* data);
void ThoughtBubble_StageLoad();
void ThoughtBubble_EditorDraw();
void ThoughtBubble_EditorLoad();
void ThoughtBubble_Serialize();

// Extra Entity Functions


#endif //!OBJ_THOUGHTBUBBLE_H
