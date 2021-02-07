#ifndef OBJ_EGGTOWER_H
#define OBJ_EGGTOWER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectEggTower : Object{

};

// Entity Class
struct EntityEggTower : Entity {

};

// Object Struct
extern ObjectEggTower *EggTower;

// Standard Entity Events
void EggTower_Update();
void EggTower_LateUpdate();
void EggTower_StaticUpdate();
void EggTower_Draw();
void EggTower_Create(void* data);
void EggTower_StageLoad();
void EggTower_EditorDraw();
void EggTower_EditorLoad();
void EggTower_Serialize();

// Extra Entity Functions


#endif //!OBJ_EGGTOWER_H
