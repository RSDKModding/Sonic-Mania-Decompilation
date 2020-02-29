#ifndef OBJ_ERZGUNNER_H
#define OBJ_ERZGUNNER_H

#include "../../SonicMania.h"

//Object Class
class ObjectERZGunner : public Object {
public:

};

//Entity Class
class EntityERZGunner : public Entity {
public:

};

//Object Entity
ObjectERZGunner ERZGunner;

//Entity Functions
void ERZGunner_Update();
void ERZGunner_EarlyUpdate();
void ERZGunner_LateUpdate();
void ERZGunner_Draw();
void ERZGunner_Setup(void* subtype);
void ERZGunner_StageLoad();
void ERZGunner_GetAttributes();

#endif //!OBJ_ERZGUNNER_H
