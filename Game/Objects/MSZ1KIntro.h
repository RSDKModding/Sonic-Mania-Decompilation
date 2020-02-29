#ifndef OBJ_MSZ1KINTRO_H
#define OBJ_MSZ1KINTRO_H

#include "../../SonicMania.h"

//Object Class
class ObjectMSZ1KIntro : public Object {
public:

};

//Entity Class
class EntityMSZ1KIntro : public Entity {
public:

};

//Object Entity
ObjectMSZ1KIntro MSZ1KIntro;

//Entity Functions
void MSZ1KIntro_Update();
void MSZ1KIntro_EarlyUpdate();
void MSZ1KIntro_LateUpdate();
void MSZ1KIntro_Draw();
void MSZ1KIntro_Setup(void* subtype);
void MSZ1KIntro_StageLoad();
void MSZ1KIntro_GetAttributes();

#endif //!OBJ_MSZ1KINTRO_H
