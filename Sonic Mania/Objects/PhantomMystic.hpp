#ifndef OBJ_PHANTOMMYSTIC_H
#define OBJ_PHANTOMMYSTIC_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPhantomMystic : Object{

};

// Entity Class
struct EntityPhantomMystic : Entity {

};

// Object Struct
extern ObjectPhantomMystic *PhantomMystic;

// Standard Entity Events
void PhantomMystic_Update();
void PhantomMystic_LateUpdate();
void PhantomMystic_StaticUpdate();
void PhantomMystic_Draw();
void PhantomMystic_Create(void* data);
void PhantomMystic_StageLoad();
void PhantomMystic_EditorDraw();
void PhantomMystic_EditorLoad();
void PhantomMystic_Serialize();

// Extra Entity Functions


#endif //!OBJ_PHANTOMMYSTIC_H
