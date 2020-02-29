#ifndef OBJ_ICEBOMBA_H
#define OBJ_ICEBOMBA_H

//Object Class
class IceBomba : Object {

};

//Entity Class
class EntityIceBomba : Entity {

};

//Entity Functions
void IceBomba_Update();
void IceBomba_EarlyUpdate();
void IceBomba_LateUpdate();
void IceBomba_Draw();
void IceBomba_Setup(void* subtype);
void IceBomba_StageLoad();
void IceBomba_GetAttributes();

#endif //!OBJ_ICEBOMBA_H
