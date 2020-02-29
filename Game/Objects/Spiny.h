#ifndef OBJ_SPINY_H
#define OBJ_SPINY_H

//Object Class
class Spiny : Object {

};

//Entity Class
class EntitySpiny : Entity {

};

//Entity Functions
void Spiny_Update();
void Spiny_EarlyUpdate();
void Spiny_LateUpdate();
void Spiny_Draw();
void Spiny_Setup(void* subtype);
void Spiny_StageLoad();
void Spiny_GetAttributes();

#endif //!OBJ_SPINY_H
