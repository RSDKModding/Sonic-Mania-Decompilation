#ifndef OBJ_LRZCONVEYOR_H
#define OBJ_LRZCONVEYOR_H

//Object Class
class LRZConveyor : Object {

};

//Entity Class
class EntityLRZConveyor : Entity {

};

//Entity Functions
void LRZConveyor_Update();
void LRZConveyor_EarlyUpdate();
void LRZConveyor_LateUpdate();
void LRZConveyor_Draw();
void LRZConveyor_Setup(void* subtype);
void LRZConveyor_StageLoad();
void LRZConveyor_GetAttributes();

#endif //!OBJ_LRZCONVEYOR_H
