#ifndef OBJ_MINE_H
#define OBJ_MINE_H

#include "../../SonicMania.h"

//Object Class
class ObjectMine : public Object {
public:

};

//Entity Class
class EntityMine : public Entity {
public:

};

//Object Entity
ObjectMine Mine;

//Entity Functions
void Mine_Update();
void Mine_EarlyUpdate();
void Mine_LateUpdate();
void Mine_Draw();
void Mine_Setup(void* subtype);
void Mine_StageLoad();
void Mine_GetAttributes();

#endif //!OBJ_MINE_H
