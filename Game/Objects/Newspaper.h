#ifndef OBJ_NEWSPAPER_H
#define OBJ_NEWSPAPER_H

#include "../../SonicMania.h"

//Object Class
class ObjectNewspaper : public Object {
public:

};

//Entity Class
class EntityNewspaper : public Entity {
public:

};

//Object Entity
ObjectNewspaper Newspaper;

//Entity Functions
void Newspaper_Update();
void Newspaper_EarlyUpdate();
void Newspaper_LateUpdate();
void Newspaper_Draw();
void Newspaper_Setup(void* subtype);
void Newspaper_StageLoad();
void Newspaper_GetAttributes();

#endif //!OBJ_NEWSPAPER_H
