#ifndef OBJ_HEAVYGUNNER_H
#define OBJ_HEAVYGUNNER_H

//Object Class
class HeavyGunner : Object {

};

//Entity Class
class EntityHeavyGunner : Entity {

};

//Entity Functions
void HeavyGunner_Update();
void HeavyGunner_EarlyUpdate();
void HeavyGunner_LateUpdate();
void HeavyGunner_Draw();
void HeavyGunner_Setup(void* subtype);
void HeavyGunner_StageLoad();
void HeavyGunner_GetAttributes();

#endif //!OBJ_HEAVYGUNNER_H
