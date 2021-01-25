#ifndef OBJ_MOTOBUG_H
#define OBJ_MOTOBUG_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectMotobug : Object {

};

// Entity Class
struct EntityMotobug : Entity {

};

// Object Entity
extern ObjectMotobug Motobug;

// Standard Entity Events
void Motobug_Update();
void Motobug_LateUpdate();
void Motobug_StaticUpdate();
void Motobug_Draw();
void Motobug_Create(void* data);
void Motobug_StageLoad();
void Motobug_EditorDraw();
void Motobug_EditorLoad();
void Motobug_Serialize();

// Extra Entity Functions


#endif //!OBJ_MOTOBUG_H
