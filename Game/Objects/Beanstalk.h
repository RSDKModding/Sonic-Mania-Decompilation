#ifndef OBJ_BEANSTALK_H
#define OBJ_BEANSTALK_H

#include "../../SonicMania.h"

//Object Class
class ObjectBeanstalk : public Object {
public:

};

//Entity Class
class EntityBeanstalk : public Entity {
public:

};

//Object Entity
ObjectBeanstalk Beanstalk;

//Entity Functions
void Beanstalk_Update();
void Beanstalk_EarlyUpdate();
void Beanstalk_LateUpdate();
void Beanstalk_Draw();
void Beanstalk_Setup(void* subtype);
void Beanstalk_StageLoad();
void Beanstalk_GetAttributes();

#endif //!OBJ_BEANSTALK_H
