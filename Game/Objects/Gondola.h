#ifndef OBJ_GONDOLA_H
#define OBJ_GONDOLA_H

//Object Class
class Gondola : Object {

};

//Entity Class
class EntityGondola : Entity {

};

//Entity Functions
void Gondola_Update();
void Gondola_EarlyUpdate();
void Gondola_LateUpdate();
void Gondola_Draw();
void Gondola_Setup(void* subtype);
void Gondola_StageLoad();
void Gondola_GetAttributes();

#endif //!OBJ_GONDOLA_H
