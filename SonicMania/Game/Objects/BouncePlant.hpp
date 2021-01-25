#ifndef OBJ_BOUNCEPLANT_H
#define OBJ_BOUNCEPLANT_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectBouncePlant : Object {

};

// Entity Class
struct EntityBouncePlant : Entity {

};

// Object Entity
extern ObjectBouncePlant BouncePlant;

// Standard Entity Events
void BouncePlant_Update();
void BouncePlant_LateUpdate();
void BouncePlant_StaticUpdate();
void BouncePlant_Draw();
void BouncePlant_Create(void* data);
void BouncePlant_StageLoad();
void BouncePlant_EditorDraw();
void BouncePlant_EditorLoad();
void BouncePlant_Serialize();

// Extra Entity Functions


#endif //!OBJ_BOUNCEPLANT_H
