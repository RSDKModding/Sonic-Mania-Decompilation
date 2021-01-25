#ifndef OBJ_CLUCKER_H
#define OBJ_CLUCKER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectClucker : Object {

};

// Entity Class
struct EntityClucker : Entity {

};

// Object Entity
extern ObjectClucker Clucker;

// Standard Entity Events
void Clucker_Update();
void Clucker_LateUpdate();
void Clucker_StaticUpdate();
void Clucker_Draw();
void Clucker_Create(void* data);
void Clucker_StageLoad();
void Clucker_EditorDraw();
void Clucker_EditorLoad();
void Clucker_Serialize();

// Extra Entity Functions


#endif //!OBJ_CLUCKER_H
