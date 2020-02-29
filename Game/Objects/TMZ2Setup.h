#ifndef OBJ_TMZ2SETUP_H
#define OBJ_TMZ2SETUP_H

//Object Class
class TMZ2Setup : Object {

};

//Entity Class
class EntityTMZ2Setup : Entity {

};

//Entity Functions
void TMZ2Setup_Update();
void TMZ2Setup_EarlyUpdate();
void TMZ2Setup_LateUpdate();
void TMZ2Setup_Draw();
void TMZ2Setup_Setup(void* subtype);
void TMZ2Setup_StageLoad();
void TMZ2Setup_GetAttributes();

#endif //!OBJ_TMZ2SETUP_H
