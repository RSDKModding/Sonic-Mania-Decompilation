#ifndef OBJ_PHANTOMKING_H
#define OBJ_PHANTOMKING_H

//Object Class
class PhantomKing : Object {

};

//Entity Class
class EntityPhantomKing : Entity {

};

//Entity Functions
void PhantomKing_Update();
void PhantomKing_EarlyUpdate();
void PhantomKing_LateUpdate();
void PhantomKing_Draw();
void PhantomKing_Setup(void* subtype);
void PhantomKing_StageLoad();
void PhantomKing_GetAttributes();

#endif //!OBJ_PHANTOMKING_H
