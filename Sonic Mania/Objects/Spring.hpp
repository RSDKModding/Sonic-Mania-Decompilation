#ifndef OBJ_SPRING_H
#define OBJ_SPRING_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSpring : Object{

};

// Entity Class
struct EntitySpring : Entity {

};

// Object Struct
extern ObjectSpring *Spring;

// Standard Entity Events
void Spring_Update();
void Spring_LateUpdate();
void Spring_StaticUpdate();
void Spring_Draw();
void Spring_Create(void* data);
void Spring_StageLoad();
void Spring_EditorDraw();
void Spring_EditorLoad();
void Spring_Serialize();

// Extra Entity Functions


#endif //!OBJ_SPRING_H
