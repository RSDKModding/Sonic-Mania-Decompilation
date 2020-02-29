#ifndef OBJ_DECORATION_H
#define OBJ_DECORATION_H

//Object Class
class Decoration : Object {

};

//Entity Class
class EntityDecoration : Entity {

};

//Entity Functions
void Decoration_Update();
void Decoration_EarlyUpdate();
void Decoration_LateUpdate();
void Decoration_Draw();
void Decoration_Setup(void* subtype);
void Decoration_StageLoad();
void Decoration_GetAttributes();

#endif //!OBJ_DECORATION_H
