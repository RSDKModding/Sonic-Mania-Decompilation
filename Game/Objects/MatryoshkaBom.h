#ifndef OBJ_MATRYOSHKABOM_H
#define OBJ_MATRYOSHKABOM_H

#include "../../SonicMania.h"

//Object Class
class ObjectMatryoshkaBom : public Object {
public:

};

//Entity Class
class EntityMatryoshkaBom : public Entity {
public:

};

//Object Entity
ObjectMatryoshkaBom MatryoshkaBom;

//Entity Functions
void MatryoshkaBom_Update();
void MatryoshkaBom_EarlyUpdate();
void MatryoshkaBom_LateUpdate();
void MatryoshkaBom_Draw();
void MatryoshkaBom_Setup(void* subtype);
void MatryoshkaBom_StageLoad();
void MatryoshkaBom_GetAttributes();

#endif //!OBJ_MATRYOSHKABOM_H
