#ifndef OBJ_PSZEGGMAN_H
#define OBJ_PSZEGGMAN_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPSZEggman : Object {

};

// Entity Class
struct EntityPSZEggman : Entity {

};

// Object Struct
extern ObjectPSZEggman *PSZEggman;

// Standard Entity Events
void PSZEggman_Update();
void PSZEggman_LateUpdate();
void PSZEggman_StaticUpdate();
void PSZEggman_Draw();
void PSZEggman_Create(void* data);
void PSZEggman_StageLoad();
void PSZEggman_EditorDraw();
void PSZEggman_EditorLoad();
void PSZEggman_Serialize();

// Extra Entity Functions


#endif //!OBJ_PSZEGGMAN_H
