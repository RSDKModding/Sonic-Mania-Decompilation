#ifndef OBJ_PHANTOMRUBY_H
#define OBJ_PHANTOMRUBY_H

//Object Class
class PhantomRuby : Object {

};

//Entity Class
class EntityPhantomRuby : Entity {

};

//Entity Functions
void PhantomRuby_Update();
void PhantomRuby_EarlyUpdate();
void PhantomRuby_LateUpdate();
void PhantomRuby_Draw();
void PhantomRuby_Setup(void* subtype);
void PhantomRuby_StageLoad();
void PhantomRuby_GetAttributes();

#endif //!OBJ_PHANTOMRUBY_H
