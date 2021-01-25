#ifndef OBJ_SYRINGE_H
#define OBJ_SYRINGE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSyringe : Object {

};

// Entity Class
struct EntitySyringe : Entity {

};

// Object Entity
extern ObjectSyringe Syringe;

// Standard Entity Events
void Syringe_Update();
void Syringe_LateUpdate();
void Syringe_StaticUpdate();
void Syringe_Draw();
void Syringe_Create(void* data);
void Syringe_StageLoad();
void Syringe_EditorDraw();
void Syringe_EditorLoad();
void Syringe_Serialize();

// Extra Entity Functions


#endif //!OBJ_SYRINGE_H
