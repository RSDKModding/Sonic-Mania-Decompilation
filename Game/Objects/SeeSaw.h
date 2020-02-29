#ifndef OBJ_SEESAW_H
#define OBJ_SEESAW_H

//Object Class
class SeeSaw : Object {

};

//Entity Class
class EntitySeeSaw : Entity {

};

//Entity Functions
void SeeSaw_Update();
void SeeSaw_EarlyUpdate();
void SeeSaw_LateUpdate();
void SeeSaw_Draw();
void SeeSaw_Setup(void* subtype);
void SeeSaw_StageLoad();
void SeeSaw_GetAttributes();

#endif //!OBJ_SEESAW_H
