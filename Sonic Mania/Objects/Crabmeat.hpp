#ifndef OBJ_CRABMEAT_H
#define OBJ_CRABMEAT_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectCrabmeat : Object {

};

// Entity Class
struct EntityCrabmeat : Entity {

};

// Object Struct
extern ObjectCrabmeat *Crabmeat;

// Standard Entity Events
void Crabmeat_Update();
void Crabmeat_LateUpdate();
void Crabmeat_StaticUpdate();
void Crabmeat_Draw();
void Crabmeat_Create(void* data);
void Crabmeat_StageLoad();
void Crabmeat_EditorDraw();
void Crabmeat_EditorLoad();
void Crabmeat_Serialize();

// Extra Entity Functions


#endif //!OBJ_CRABMEAT_H
