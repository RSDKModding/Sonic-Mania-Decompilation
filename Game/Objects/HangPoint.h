#ifndef OBJ_HANGPOINT_H
#define OBJ_HANGPOINT_H

#include "../../SonicMania.h"

//Object Class
class ObjectHangPoint : public Object {
public:

};

//Entity Class
class EntityHangPoint : public Entity {
public:

};

//Object Entity
ObjectHangPoint HangPoint;

//Entity Functions
void HangPoint_Update();
void HangPoint_EarlyUpdate();
void HangPoint_LateUpdate();
void HangPoint_Draw();
void HangPoint_Setup(void* subtype);
void HangPoint_StageLoad();
void HangPoint_GetAttributes();

#endif //!OBJ_HANGPOINT_H
