#ifndef OBJ_LRZ2SETUP_H
#define OBJ_LRZ2SETUP_H

//Object Class
class LRZ2Setup : Object {

};

//Entity Class
class EntityLRZ2Setup : Entity {

};

//Entity Functions
void LRZ2Setup_Update();
void LRZ2Setup_EarlyUpdate();
void LRZ2Setup_LateUpdate();
void LRZ2Setup_Draw();
void LRZ2Setup_Setup(void* subtype);
void LRZ2Setup_StageLoad();
void LRZ2Setup_GetAttributes();

#endif //!OBJ_LRZ2SETUP_H
