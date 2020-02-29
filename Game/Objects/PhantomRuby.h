#ifndef OBJ_PHANTOMRUBY_H
#define OBJ_PHANTOMRUBY_H

#include "../../SonicMania.h"

//Object Class
class ObjectPhantomRuby : public Object {
public:

};

//Entity Class
class EntityPhantomRuby : public Entity {
public:

};

//Object Entity
ObjectPhantomRuby PhantomRuby;

//Entity Functions
void PhantomRuby_Update();
void PhantomRuby_EarlyUpdate();
void PhantomRuby_LateUpdate();
void PhantomRuby_Draw();
void PhantomRuby_Setup(void* subtype);
void PhantomRuby_StageLoad();
void PhantomRuby_GetAttributes();

#endif //!OBJ_PHANTOMRUBY_H
