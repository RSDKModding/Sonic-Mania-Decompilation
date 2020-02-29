#ifndef OBJ_POINTDEXTER_H
#define OBJ_POINTDEXTER_H

//Object Class
class Pointdexter : Object {

};

//Entity Class
class EntityPointdexter : Entity {

};

//Entity Functions
void Pointdexter_Update();
void Pointdexter_EarlyUpdate();
void Pointdexter_LateUpdate();
void Pointdexter_Draw();
void Pointdexter_Setup(void* subtype);
void Pointdexter_StageLoad();
void Pointdexter_GetAttributes();

#endif //!OBJ_POINTDEXTER_H
