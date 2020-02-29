#ifndef OBJ_AQUIS_H
#define OBJ_AQUIS_H

//Object Class
class Aquis : Object {

};

//Entity Class
class EntityAquis : Entity {

};

//Entity Functions
void Aquis_Update();
void Aquis_EarlyUpdate();
void Aquis_LateUpdate();
void Aquis_Draw();
void Aquis_Setup(void* subtype);
void Aquis_StageLoad();
void Aquis_GetAttributes();

#endif //!OBJ_AQUIS_H
