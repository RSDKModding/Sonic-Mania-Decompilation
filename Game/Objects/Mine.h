#ifndef OBJ_MINE_H
#define OBJ_MINE_H

//Object Class
class Mine : Object {

};

//Entity Class
class EntityMine : Entity {

};

//Entity Functions
void Mine_Update();
void Mine_EarlyUpdate();
void Mine_LateUpdate();
void Mine_Draw();
void Mine_Setup(void* subtype);
void Mine_StageLoad();
void Mine_GetAttributes();

#endif //!OBJ_MINE_H
