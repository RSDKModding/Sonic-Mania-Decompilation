#ifndef OBJ_PRESS_H
#define OBJ_PRESS_H

#include "../../SonicMania.h"

//Object Class
class ObjectPress : public Object {
public:

};

//Entity Class
class EntityPress : public Entity {
public:

};

//Object Entity
ObjectPress Press;

//Entity Functions
void Press_Update();
void Press_EarlyUpdate();
void Press_LateUpdate();
void Press_Draw();
void Press_Setup(void* subtype);
void Press_StageLoad();
void Press_GetAttributes();

#endif //!OBJ_PRESS_H
