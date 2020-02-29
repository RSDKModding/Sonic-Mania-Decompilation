#ifndef OBJ_KLEPTOMOBILE_H
#define OBJ_KLEPTOMOBILE_H

//Object Class
class KleptoMobile : Object {

};

//Entity Class
class EntityKleptoMobile : Entity {

};

//Entity Functions
void KleptoMobile_Update();
void KleptoMobile_EarlyUpdate();
void KleptoMobile_LateUpdate();
void KleptoMobile_Draw();
void KleptoMobile_Setup(void* subtype);
void KleptoMobile_StageLoad();
void KleptoMobile_GetAttributes();

#endif //!OBJ_KLEPTOMOBILE_H
