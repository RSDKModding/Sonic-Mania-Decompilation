#ifndef OBJ_HATTERKILLER_H
#define OBJ_HATTERKILLER_H

//Object Class
class Hatterkiller : Object {

};

//Entity Class
class EntityHatterkiller : Entity {

};

//Entity Functions
void Hatterkiller_Update();
void Hatterkiller_EarlyUpdate();
void Hatterkiller_LateUpdate();
void Hatterkiller_Draw();
void Hatterkiller_Setup(void* subtype);
void Hatterkiller_StageLoad();
void Hatterkiller_GetAttributes();

#endif //!OBJ_HATTERKILLER_H
