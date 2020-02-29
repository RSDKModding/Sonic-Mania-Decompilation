#ifndef OBJ_LRZSPIRAL_H
#define OBJ_LRZSPIRAL_H

//Object Class
class LRZSpiral : Object {

};

//Entity Class
class EntityLRZSpiral : Entity {

};

//Entity Functions
void LRZSpiral_Update();
void LRZSpiral_EarlyUpdate();
void LRZSpiral_LateUpdate();
void LRZSpiral_Draw();
void LRZSpiral_Setup(void* subtype);
void LRZSpiral_StageLoad();
void LRZSpiral_GetAttributes();

#endif //!OBJ_LRZSPIRAL_H
