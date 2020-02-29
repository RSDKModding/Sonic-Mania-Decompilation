#ifndef OBJ_TMZCABLE_H
#define OBJ_TMZCABLE_H

#include "../../SonicMania.h"

//Object Class
class ObjectTMZCable : public Object {
public:

};

//Entity Class
class EntityTMZCable : public Entity {
public:

};

//Object Entity
ObjectTMZCable TMZCable;

//Entity Functions
void TMZCable_Update();
void TMZCable_EarlyUpdate();
void TMZCable_LateUpdate();
void TMZCable_Draw();
void TMZCable_Setup(void* subtype);
void TMZCable_StageLoad();
void TMZCable_GetAttributes();

#endif //!OBJ_TMZCABLE_H
