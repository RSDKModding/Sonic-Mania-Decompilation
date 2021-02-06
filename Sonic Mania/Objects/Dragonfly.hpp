#ifndef OBJ_DRAGONFLY_H
#define OBJ_DRAGONFLY_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectDragonfly : Object {

};

// Entity Class
struct EntityDragonfly : Entity {

};

// Object Entity
extern ObjectDragonfly Dragonfly;

// Standard Entity Events
void Dragonfly_Update();
void Dragonfly_LateUpdate();
void Dragonfly_StaticUpdate();
void Dragonfly_Draw();
void Dragonfly_Create(void* data);
void Dragonfly_StageLoad();
void Dragonfly_EditorDraw();
void Dragonfly_EditorLoad();
void Dragonfly_Serialize();

// Extra Entity Functions


#endif //!OBJ_DRAGONFLY_H
