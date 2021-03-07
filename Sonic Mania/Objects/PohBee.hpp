#ifndef OBJ_POHBEE_H
#define OBJ_POHBEE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPohBee : Object {

};

// Entity Class
struct EntityPohBee : Entity {

};

// Object Struct
extern ObjectPohBee *PohBee;

// Standard Entity Events
void PohBee_Update();
void PohBee_LateUpdate();
void PohBee_StaticUpdate();
void PohBee_Draw();
void PohBee_Create(void* data);
void PohBee_StageLoad();
void PohBee_EditorDraw();
void PohBee_EditorLoad();
void PohBee_Serialize();

// Extra Entity Functions


#endif //!OBJ_POHBEE_H
