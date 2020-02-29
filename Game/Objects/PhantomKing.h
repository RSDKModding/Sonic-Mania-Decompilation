#ifndef OBJ_PHANTOMKING_H
#define OBJ_PHANTOMKING_H

#include "../../SonicMania.h"

//Object Class
class ObjectPhantomKing : public Object {
public:

};

//Entity Class
class EntityPhantomKing : public Entity {
public:

};

//Object Entity
ObjectPhantomKing PhantomKing;

//Entity Functions
void PhantomKing_Update();
void PhantomKing_EarlyUpdate();
void PhantomKing_LateUpdate();
void PhantomKing_Draw();
void PhantomKing_Setup(void* subtype);
void PhantomKing_StageLoad();
void PhantomKing_GetAttributes();

#endif //!OBJ_PHANTOMKING_H
