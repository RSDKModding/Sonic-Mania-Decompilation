#ifndef OBJ_CRANE_H
#define OBJ_CRANE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectCrane : Object {

};

// Entity Class
struct EntityCrane : Entity {

};

// Object Entity
extern ObjectCrane Crane;

// Standard Entity Events
void Crane_Update();
void Crane_LateUpdate();
void Crane_StaticUpdate();
void Crane_Draw();
void Crane_Create(void* data);
void Crane_StageLoad();
void Crane_EditorDraw();
void Crane_EditorLoad();
void Crane_Serialize();

// Extra Entity Functions


#endif //!OBJ_CRANE_H
