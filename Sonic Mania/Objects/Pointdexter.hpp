#ifndef OBJ_POINTDEXTER_H
#define OBJ_POINTDEXTER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPointdexter : Object {

};

// Entity Class
struct EntityPointdexter : Entity {

};

// Object Struct
extern ObjectPointdexter *Pointdexter;

// Standard Entity Events
void Pointdexter_Update();
void Pointdexter_LateUpdate();
void Pointdexter_StaticUpdate();
void Pointdexter_Draw();
void Pointdexter_Create(void* data);
void Pointdexter_StageLoad();
void Pointdexter_EditorDraw();
void Pointdexter_EditorLoad();
void Pointdexter_Serialize();

// Extra Entity Functions


#endif //!OBJ_POINTDEXTER_H
