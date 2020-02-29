#ifndef OBJ_DACONTROL_H
#define OBJ_DACONTROL_H

#include "../../SonicMania.h"

//Object Class
class ObjectDAControl : public Object {
public:

};

//Entity Class
class EntityDAControl : public Entity {
public:

};

//Object Entity
ObjectDAControl DAControl;

//Entity Functions
void DAControl_Update();
void DAControl_EarlyUpdate();
void DAControl_LateUpdate();
void DAControl_Draw();
void DAControl_Setup(void* subtype);
void DAControl_StageLoad();
void DAControl_GetAttributes();

#endif //!OBJ_DACONTROL_H
