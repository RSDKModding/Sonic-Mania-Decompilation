#ifndef OBJ_BIGSQUEEZE_H
#define OBJ_BIGSQUEEZE_H

//Object Class
class BigSqueeze : Object {

};

//Entity Class
class EntityBigSqueeze : Entity {

};

//Entity Functions
void BigSqueeze_Update();
void BigSqueeze_EarlyUpdate();
void BigSqueeze_LateUpdate();
void BigSqueeze_Draw();
void BigSqueeze_Setup(void* subtype);
void BigSqueeze_StageLoad();
void BigSqueeze_GetAttributes();

#endif //!OBJ_BIGSQUEEZE_H
