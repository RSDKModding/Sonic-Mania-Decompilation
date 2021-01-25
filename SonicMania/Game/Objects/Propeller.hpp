#ifndef OBJ_PROPELLER_H
#define OBJ_PROPELLER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPropeller : Object {

};

// Entity Class
struct EntityPropeller : Entity {

};

// Object Entity
extern ObjectPropeller Propeller;

// Standard Entity Events
void Propeller_Update();
void Propeller_LateUpdate();
void Propeller_StaticUpdate();
void Propeller_Draw();
void Propeller_Create(void* data);
void Propeller_StageLoad();
void Propeller_EditorDraw();
void Propeller_EditorLoad();
void Propeller_Serialize();

// Extra Entity Functions


#endif //!OBJ_PROPELLER_H
