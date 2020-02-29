#ifndef OBJ_ELECTROMAGNET_H
#define OBJ_ELECTROMAGNET_H

//Object Class
class ElectroMagnet : Object {

};

//Entity Class
class EntityElectroMagnet : Entity {

};

//Entity Functions
void ElectroMagnet_Update();
void ElectroMagnet_EarlyUpdate();
void ElectroMagnet_LateUpdate();
void ElectroMagnet_Draw();
void ElectroMagnet_Setup(void* subtype);
void ElectroMagnet_StageLoad();
void ElectroMagnet_GetAttributes();

#endif //!OBJ_ELECTROMAGNET_H
