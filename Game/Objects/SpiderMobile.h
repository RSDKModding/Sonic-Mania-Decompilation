#ifndef OBJ_SPIDERMOBILE_H
#define OBJ_SPIDERMOBILE_H

#include "../../SonicMania.h"

//Object Class
class ObjectSpiderMobile : public Object {
public:

};

//Entity Class
class EntitySpiderMobile : public Entity {
public:

};

//Object Entity
ObjectSpiderMobile SpiderMobile;

//Entity Functions
void SpiderMobile_Update();
void SpiderMobile_EarlyUpdate();
void SpiderMobile_LateUpdate();
void SpiderMobile_Draw();
void SpiderMobile_Setup(void* subtype);
void SpiderMobile_StageLoad();
void SpiderMobile_GetAttributes();

#endif //!OBJ_SPIDERMOBILE_H
