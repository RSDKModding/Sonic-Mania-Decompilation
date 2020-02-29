#ifndef OBJ_SHURIKEN_H
#define OBJ_SHURIKEN_H

//Object Class
class Shuriken : Object {

};

//Entity Class
class EntityShuriken : Entity {

};

//Entity Functions
void Shuriken_Update();
void Shuriken_EarlyUpdate();
void Shuriken_LateUpdate();
void Shuriken_Draw();
void Shuriken_Setup(void* subtype);
void Shuriken_StageLoad();
void Shuriken_GetAttributes();

#endif //!OBJ_SHURIKEN_H
