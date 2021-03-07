#ifndef OBJ_MINE_H
#define OBJ_MINE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectMine : Object {

};

// Entity Class
struct EntityMine : Entity {

};

// Object Struct
extern ObjectMine *Mine;

// Standard Entity Events
void Mine_Update();
void Mine_LateUpdate();
void Mine_StaticUpdate();
void Mine_Draw();
void Mine_Create(void* data);
void Mine_StageLoad();
void Mine_EditorDraw();
void Mine_EditorLoad();
void Mine_Serialize();

// Extra Entity Functions


#endif //!OBJ_MINE_H
