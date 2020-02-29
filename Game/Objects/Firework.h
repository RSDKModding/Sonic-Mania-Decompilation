#ifndef OBJ_FIREWORK_H
#define OBJ_FIREWORK_H

//Object Class
class Firework : Object {

};

//Entity Class
class EntityFirework : Entity {

};

//Entity Functions
void Firework_Update();
void Firework_EarlyUpdate();
void Firework_LateUpdate();
void Firework_Draw();
void Firework_Setup(void* subtype);
void Firework_StageLoad();
void Firework_GetAttributes();

#endif //!OBJ_FIREWORK_H
