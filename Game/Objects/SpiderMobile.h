#ifndef OBJ_SPIDERMOBILE_H
#define OBJ_SPIDERMOBILE_H

//Object Class
class SpiderMobile : Object {

};

//Entity Class
class EntitySpiderMobile : Entity {

};

//Entity Functions
void SpiderMobile_Update();
void SpiderMobile_EarlyUpdate();
void SpiderMobile_LateUpdate();
void SpiderMobile_Draw();
void SpiderMobile_Setup(void* subtype);
void SpiderMobile_StageLoad();
void SpiderMobile_GetAttributes();

#endif //!OBJ_SPIDERMOBILE_H
