#ifndef OBJ_BLASTOID_H
#define OBJ_BLASTOID_H

//Object Class
class Blastoid : Object {

};

//Entity Class
class EntityBlastoid : Entity {

};

//Entity Functions
void Blastoid_Update();
void Blastoid_EarlyUpdate();
void Blastoid_LateUpdate();
void Blastoid_Draw();
void Blastoid_Setup(void* subtype);
void Blastoid_StageLoad();
void Blastoid_GetAttributes();

#endif //!OBJ_BLASTOID_H
