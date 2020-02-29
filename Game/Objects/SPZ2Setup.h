#ifndef OBJ_SPZ2SETUP_H
#define OBJ_SPZ2SETUP_H

//Object Class
class SPZ2Setup : Object {

};

//Entity Class
class EntitySPZ2Setup : Entity {

};

//Entity Functions
void SPZ2Setup_Update();
void SPZ2Setup_EarlyUpdate();
void SPZ2Setup_LateUpdate();
void SPZ2Setup_Draw();
void SPZ2Setup_Setup(void* subtype);
void SPZ2Setup_StageLoad();
void SPZ2Setup_GetAttributes();

#endif //!OBJ_SPZ2SETUP_H
