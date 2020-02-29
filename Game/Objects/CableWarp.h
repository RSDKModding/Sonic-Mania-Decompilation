#ifndef OBJ_CABLEWARP_H
#define OBJ_CABLEWARP_H

//Object Class
class CableWarp : Object {

};

//Entity Class
class EntityCableWarp : Entity {

};

//Entity Functions
void CableWarp_Update();
void CableWarp_EarlyUpdate();
void CableWarp_LateUpdate();
void CableWarp_Draw();
void CableWarp_Setup(void* subtype);
void CableWarp_StageLoad();
void CableWarp_GetAttributes();

#endif //!OBJ_CABLEWARP_H
