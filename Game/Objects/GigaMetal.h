#ifndef OBJ_GIGAMETAL_H
#define OBJ_GIGAMETAL_H

//Object Class
class GigaMetal : Object {

};

//Entity Class
class EntityGigaMetal : Entity {

};

//Entity Functions
void GigaMetal_Update();
void GigaMetal_EarlyUpdate();
void GigaMetal_LateUpdate();
void GigaMetal_Draw();
void GigaMetal_Setup(void* subtype);
void GigaMetal_StageLoad();
void GigaMetal_GetAttributes();

#endif //!OBJ_GIGAMETAL_H
