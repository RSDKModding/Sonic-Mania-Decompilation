#ifndef OBJ_SSZEGGMAN_H
#define OBJ_SSZEGGMAN_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSSZEggman : Object {

};

// Entity Class
struct EntitySSZEggman : Entity {

};

// Object Struct
extern ObjectSSZEggman *SSZEggman;

// Standard Entity Events
void SSZEggman_Update();
void SSZEggman_LateUpdate();
void SSZEggman_StaticUpdate();
void SSZEggman_Draw();
void SSZEggman_Create(void* data);
void SSZEggman_StageLoad();
void SSZEggman_EditorDraw();
void SSZEggman_EditorLoad();
void SSZEggman_Serialize();

// Extra Entity Functions


#endif //!OBJ_SSZEGGMAN_H
