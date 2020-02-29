#ifndef OBJ_PSZ2SETUP_H
#define OBJ_PSZ2SETUP_H

//Object Class
class PSZ2Setup : Object {

};

//Entity Class
class EntityPSZ2Setup : Entity {

};

//Entity Functions
void PSZ2Setup_Update();
void PSZ2Setup_EarlyUpdate();
void PSZ2Setup_LateUpdate();
void PSZ2Setup_Draw();
void PSZ2Setup_Setup(void* subtype);
void PSZ2Setup_StageLoad();
void PSZ2Setup_GetAttributes();

#endif //!OBJ_PSZ2SETUP_H
