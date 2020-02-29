#ifndef OBJ_THOUGHTBUBBLE_H
#define OBJ_THOUGHTBUBBLE_H

//Object Class
class ThoughtBubble : Object {

};

//Entity Class
class EntityThoughtBubble : Entity {

};

//Entity Functions
void ThoughtBubble_Update();
void ThoughtBubble_EarlyUpdate();
void ThoughtBubble_LateUpdate();
void ThoughtBubble_Draw();
void ThoughtBubble_Setup(void* subtype);
void ThoughtBubble_StageLoad();
void ThoughtBubble_GetAttributes();

#endif //!OBJ_THOUGHTBUBBLE_H
