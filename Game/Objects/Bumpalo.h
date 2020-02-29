#ifndef OBJ_BUMPALO_H
#define OBJ_BUMPALO_H

//Object Class
class Bumpalo : Object {

};

//Entity Class
class EntityBumpalo : Entity {

};

//Entity Functions
void Bumpalo_Update();
void Bumpalo_EarlyUpdate();
void Bumpalo_LateUpdate();
void Bumpalo_Draw();
void Bumpalo_Setup(void* subtype);
void Bumpalo_StageLoad();
void Bumpalo_GetAttributes();

#endif //!OBJ_BUMPALO_H
