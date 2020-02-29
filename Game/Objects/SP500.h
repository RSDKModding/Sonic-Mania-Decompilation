#ifndef OBJ_SP500_H
#define OBJ_SP500_H

//Object Class
class SP500 : Object {

};

//Entity Class
class EntitySP500 : Entity {

};

//Entity Functions
void SP500_Update();
void SP500_EarlyUpdate();
void SP500_LateUpdate();
void SP500_Draw();
void SP500_Setup(void* subtype);
void SP500_StageLoad();
void SP500_GetAttributes();

#endif //!OBJ_SP500_H
