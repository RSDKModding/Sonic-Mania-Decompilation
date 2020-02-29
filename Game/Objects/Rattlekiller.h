#ifndef OBJ_RATTLEKILLER_H
#define OBJ_RATTLEKILLER_H

//Object Class
class Rattlekiller : Object {

};

//Entity Class
class EntityRattlekiller : Entity {

};

//Entity Functions
void Rattlekiller_Update();
void Rattlekiller_EarlyUpdate();
void Rattlekiller_LateUpdate();
void Rattlekiller_Draw();
void Rattlekiller_Setup(void* subtype);
void Rattlekiller_StageLoad();
void Rattlekiller_GetAttributes();

#endif //!OBJ_RATTLEKILLER_H
