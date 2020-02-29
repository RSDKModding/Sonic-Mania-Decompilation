#ifndef OBJ_MATRYOSHKABOM_H
#define OBJ_MATRYOSHKABOM_H

//Object Class
class MatryoshkaBom : Object {

};

//Entity Class
class EntityMatryoshkaBom : Entity {

};

//Entity Functions
void MatryoshkaBom_Update();
void MatryoshkaBom_EarlyUpdate();
void MatryoshkaBom_LateUpdate();
void MatryoshkaBom_Draw();
void MatryoshkaBom_Setup(void* subtype);
void MatryoshkaBom_StageLoad();
void MatryoshkaBom_GetAttributes();

#endif //!OBJ_MATRYOSHKABOM_H
