#ifndef OBJ_ERZGUNNER_H
#define OBJ_ERZGUNNER_H

//Object Class
class ERZGunner : Object {

};

//Entity Class
class EntityERZGunner : Entity {

};

//Entity Functions
void ERZGunner_Update();
void ERZGunner_EarlyUpdate();
void ERZGunner_LateUpdate();
void ERZGunner_Draw();
void ERZGunner_Setup(void* subtype);
void ERZGunner_StageLoad();
void ERZGunner_GetAttributes();

#endif //!OBJ_ERZGUNNER_H
