#ifndef OBJ_SSZ2SETUP_H
#define OBJ_SSZ2SETUP_H

//Object Class
class SSZ2Setup : Object {

};

//Entity Class
class EntitySSZ2Setup : Entity {

};

//Entity Functions
void SSZ2Setup_Update();
void SSZ2Setup_EarlyUpdate();
void SSZ2Setup_LateUpdate();
void SSZ2Setup_Draw();
void SSZ2Setup_Setup(void* subtype);
void SSZ2Setup_StageLoad();
void SSZ2Setup_GetAttributes();

#endif //!OBJ_SSZ2SETUP_H
