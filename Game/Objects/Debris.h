#ifndef OBJ_DEBRIS_H
#define OBJ_DEBRIS_H

//Object Class
class Debris : Object {

};

//Entity Class
class EntityDebris : Entity {

};

//Entity Functions
void Debris_Update();
void Debris_EarlyUpdate();
void Debris_LateUpdate();
void Debris_Draw();
void Debris_Setup(void* subtype);
void Debris_StageLoad();
void Debris_GetAttributes();

#endif //!OBJ_DEBRIS_H
