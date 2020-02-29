#ifndef OBJ_JAWZ_H
#define OBJ_JAWZ_H

//Object Class
class Jawz : Object {

};

//Entity Class
class EntityJawz : Entity {

};

//Entity Functions
void Jawz_Update();
void Jawz_EarlyUpdate();
void Jawz_LateUpdate();
void Jawz_Draw();
void Jawz_Setup(void* subtype);
void Jawz_StageLoad();
void Jawz_GetAttributes();

#endif //!OBJ_JAWZ_H
