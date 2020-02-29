#ifndef OBJ_BUZZSAW_H
#define OBJ_BUZZSAW_H

//Object Class
class BuzzSaw : Object {

};

//Entity Class
class EntityBuzzSaw : Entity {

};

//Entity Functions
void BuzzSaw_Update();
void BuzzSaw_EarlyUpdate();
void BuzzSaw_LateUpdate();
void BuzzSaw_Draw();
void BuzzSaw_Setup(void* subtype);
void BuzzSaw_StageLoad();
void BuzzSaw_GetAttributes();

#endif //!OBJ_BUZZSAW_H
