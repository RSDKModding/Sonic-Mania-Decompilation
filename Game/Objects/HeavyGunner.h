#ifndef OBJ_HEAVYGUNNER_H
#define OBJ_HEAVYGUNNER_H

#include "../../SonicMania.h"

//Object Class
class ObjectHeavyGunner : public Object {
public:

};

//Entity Class
class EntityHeavyGunner : public Entity {
public:

};

//Object Entity
ObjectHeavyGunner HeavyGunner;

//Entity Functions
void HeavyGunner_Update();
void HeavyGunner_EarlyUpdate();
void HeavyGunner_LateUpdate();
void HeavyGunner_Draw();
void HeavyGunner_Setup(void* subtype);
void HeavyGunner_StageLoad();
void HeavyGunner_GetAttributes();

#endif //!OBJ_HEAVYGUNNER_H
