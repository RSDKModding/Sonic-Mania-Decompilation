#ifndef OBJ_CIRCLEBUMPER_H
#define OBJ_CIRCLEBUMPER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectCircleBumper : Object {

};

// Entity Class
struct EntityCircleBumper : Entity {

};

// Object Entity
extern ObjectCircleBumper CircleBumper;

// Standard Entity Events
void CircleBumper_Update();
void CircleBumper_LateUpdate();
void CircleBumper_StaticUpdate();
void CircleBumper_Draw();
void CircleBumper_Create(void* data);
void CircleBumper_StageLoad();
void CircleBumper_EditorDraw();
void CircleBumper_EditorLoad();
void CircleBumper_Serialize();

// Extra Entity Functions


#endif //!OBJ_CIRCLEBUMPER_H
