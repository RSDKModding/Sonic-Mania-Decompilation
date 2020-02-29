#ifndef OBJ_JAWZ_H
#define OBJ_JAWZ_H

#include "../../SonicMania.h"

//Object Class
class ObjectJawz : public Object {
public:

};

//Entity Class
class EntityJawz : public Entity {
public:

};

//Object Entity
ObjectJawz Jawz;

//Entity Functions
void Jawz_Update();
void Jawz_EarlyUpdate();
void Jawz_LateUpdate();
void Jawz_Draw();
void Jawz_Setup(void* subtype);
void Jawz_StageLoad();
void Jawz_GetAttributes();

#endif //!OBJ_JAWZ_H
