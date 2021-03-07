#ifndef OBJ_EGGMAN_H
#define OBJ_EGGMAN_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectEggman : Object {

};

// Entity Class
struct EntityEggman : Entity {

};

// Object Struct
extern ObjectEggman *Eggman;

// Standard Entity Events
void Eggman_Update();
void Eggman_LateUpdate();
void Eggman_StaticUpdate();
void Eggman_Draw();
void Eggman_Create(void* data);
void Eggman_StageLoad();
void Eggman_EditorDraw();
void Eggman_EditorLoad();
void Eggman_Serialize();

// Extra Entity Functions


#endif //!OBJ_EGGMAN_H
