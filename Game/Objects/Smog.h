#ifndef OBJ_SMOG_H
#define OBJ_SMOG_H

//Object Class
class Smog : Object {

};

//Entity Class
class EntitySmog : Entity {

};

//Entity Functions
void Smog_Update();
void Smog_EarlyUpdate();
void Smog_LateUpdate();
void Smog_Draw();
void Smog_Setup(void* subtype);
void Smog_StageLoad();
void Smog_GetAttributes();

#endif //!OBJ_SMOG_H
