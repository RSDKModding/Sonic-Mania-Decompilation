#ifndef OBJ_BATBRAIN_H
#define OBJ_BATBRAIN_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectBatbrain : Object{

};

// Entity Class
struct EntityBatbrain : Entity {

};

// Object Struct
extern ObjectBatbrain *Batbrain;

// Standard Entity Events
void Batbrain_Update();
void Batbrain_LateUpdate();
void Batbrain_StaticUpdate();
void Batbrain_Draw();
void Batbrain_Create(void* data);
void Batbrain_StageLoad();
void Batbrain_EditorDraw();
void Batbrain_EditorLoad();
void Batbrain_Serialize();

// Extra Entity Functions


#endif //!OBJ_BATBRAIN_H
