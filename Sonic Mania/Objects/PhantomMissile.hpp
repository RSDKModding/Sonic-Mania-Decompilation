#ifndef OBJ_PHANTOMMISSILE_H
#define OBJ_PHANTOMMISSILE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPhantomMissile : Object {

};

// Entity Class
struct EntityPhantomMissile : Entity {

};

// Object Struct
extern ObjectPhantomMissile *PhantomMissile;

// Standard Entity Events
void PhantomMissile_Update();
void PhantomMissile_LateUpdate();
void PhantomMissile_StaticUpdate();
void PhantomMissile_Draw();
void PhantomMissile_Create(void* data);
void PhantomMissile_StageLoad();
void PhantomMissile_EditorDraw();
void PhantomMissile_EditorLoad();
void PhantomMissile_Serialize();

// Extra Entity Functions


#endif //!OBJ_PHANTOMMISSILE_H
