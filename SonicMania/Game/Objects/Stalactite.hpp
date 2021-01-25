#ifndef OBJ_STALACTITE_H
#define OBJ_STALACTITE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectStalactite : Object {

};

// Entity Class
struct EntityStalactite : Entity {

};

// Object Entity
extern ObjectStalactite Stalactite;

// Standard Entity Events
void Stalactite_Update();
void Stalactite_LateUpdate();
void Stalactite_StaticUpdate();
void Stalactite_Draw();
void Stalactite_Create(void* data);
void Stalactite_StageLoad();
void Stalactite_EditorDraw();
void Stalactite_EditorLoad();
void Stalactite_Serialize();

// Extra Entity Functions


#endif //!OBJ_STALACTITE_H
