#ifndef OBJ_MECHABU_H
#define OBJ_MECHABU_H

//Object Class
class MechaBu : Object {

};

//Entity Class
class EntityMechaBu : Entity {

};

//Entity Functions
void MechaBu_Update();
void MechaBu_EarlyUpdate();
void MechaBu_LateUpdate();
void MechaBu_Draw();
void MechaBu_Setup(void* subtype);
void MechaBu_StageLoad();
void MechaBu_GetAttributes();

#endif //!OBJ_MECHABU_H
