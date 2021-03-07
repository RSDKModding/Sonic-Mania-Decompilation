#ifndef OBJ_ESCAPECAR_H
#define OBJ_ESCAPECAR_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectEscapeCar : Object {

};

// Entity Class
struct EntityEscapeCar : Entity {

};

// Object Struct
extern ObjectEscapeCar *EscapeCar;

// Standard Entity Events
void EscapeCar_Update();
void EscapeCar_LateUpdate();
void EscapeCar_StaticUpdate();
void EscapeCar_Draw();
void EscapeCar_Create(void* data);
void EscapeCar_StageLoad();
void EscapeCar_EditorDraw();
void EscapeCar_EditorLoad();
void EscapeCar_Serialize();

// Extra Entity Functions


#endif //!OBJ_ESCAPECAR_H
