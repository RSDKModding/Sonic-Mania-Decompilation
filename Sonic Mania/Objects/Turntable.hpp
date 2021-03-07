#ifndef OBJ_TURNTABLE_H
#define OBJ_TURNTABLE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectTurntable : Object {

};

// Entity Class
struct EntityTurntable : Entity {

};

// Object Struct
extern ObjectTurntable *Turntable;

// Standard Entity Events
void Turntable_Update();
void Turntable_LateUpdate();
void Turntable_StaticUpdate();
void Turntable_Draw();
void Turntable_Create(void* data);
void Turntable_StageLoad();
void Turntable_EditorDraw();
void Turntable_EditorLoad();
void Turntable_Serialize();

// Extra Entity Functions


#endif //!OBJ_TURNTABLE_H
