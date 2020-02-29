#ifndef OBJ_PISTON_H
#define OBJ_PISTON_H

//Object Class
class Piston : Object {

};

//Entity Class
class EntityPiston : Entity {

};

//Entity Functions
void Piston_Update();
void Piston_EarlyUpdate();
void Piston_LateUpdate();
void Piston_Draw();
void Piston_Setup(void* subtype);
void Piston_StageLoad();
void Piston_GetAttributes();

#endif //!OBJ_PISTON_H
