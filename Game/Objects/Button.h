#ifndef OBJ_BUTTON_H
#define OBJ_BUTTON_H

#include "../../SonicMania.h"

//Object Class
class ObjectButton : public Object {
public:

};

//Entity Class
class EntityButton : public Entity {
public:

};

//Object Entity
ObjectButton Button;

//Entity Functions
void Button_Update();
void Button_EarlyUpdate();
void Button_LateUpdate();
void Button_Draw();
void Button_Setup(void* subtype);
void Button_StageLoad();
void Button_GetAttributes();

#endif //!OBJ_BUTTON_H
