#ifndef OBJ_THOUGHTBUBBLE_H
#define OBJ_THOUGHTBUBBLE_H

#include "../../SonicMania.h"

//Object Class
class ObjectThoughtBubble : public Object {
public:

};

//Entity Class
class EntityThoughtBubble : public Entity {
public:

};

//Object Entity
ObjectThoughtBubble ThoughtBubble;

//Entity Functions
void ThoughtBubble_Update();
void ThoughtBubble_EarlyUpdate();
void ThoughtBubble_LateUpdate();
void ThoughtBubble_Draw();
void ThoughtBubble_Setup(void* subtype);
void ThoughtBubble_StageLoad();
void ThoughtBubble_GetAttributes();

#endif //!OBJ_THOUGHTBUBBLE_H
