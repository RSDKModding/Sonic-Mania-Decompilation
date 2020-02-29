#ifndef OBJ_TUBINAUT_H
#define OBJ_TUBINAUT_H

//Object Class
class Tubinaut : Object {

};

//Entity Class
class EntityTubinaut : Entity {

};

//Entity Functions
void Tubinaut_Update();
void Tubinaut_EarlyUpdate();
void Tubinaut_LateUpdate();
void Tubinaut_Draw();
void Tubinaut_Setup(void* subtype);
void Tubinaut_StageLoad();
void Tubinaut_GetAttributes();

#endif //!OBJ_TUBINAUT_H
