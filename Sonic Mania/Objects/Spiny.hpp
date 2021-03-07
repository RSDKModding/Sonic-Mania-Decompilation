#ifndef OBJ_SPINY_H
#define OBJ_SPINY_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSpiny : Object {

};

// Entity Class
struct EntitySpiny : Entity {

};

// Object Struct
extern ObjectSpiny *Spiny;

// Standard Entity Events
void Spiny_Update();
void Spiny_LateUpdate();
void Spiny_StaticUpdate();
void Spiny_Draw();
void Spiny_Create(void* data);
void Spiny_StageLoad();
void Spiny_EditorDraw();
void Spiny_EditorLoad();
void Spiny_Serialize();

// Extra Entity Functions


#endif //!OBJ_SPINY_H
