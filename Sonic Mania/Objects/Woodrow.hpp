#ifndef OBJ_WOODROW_H
#define OBJ_WOODROW_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectWoodrow : Object {

};

// Entity Class
struct EntityWoodrow : Entity {

};

// Object Struct
extern ObjectWoodrow *Woodrow;

// Standard Entity Events
void Woodrow_Update();
void Woodrow_LateUpdate();
void Woodrow_StaticUpdate();
void Woodrow_Draw();
void Woodrow_Create(void* data);
void Woodrow_StageLoad();
void Woodrow_EditorDraw();
void Woodrow_EditorLoad();
void Woodrow_Serialize();

// Extra Entity Functions


#endif //!OBJ_WOODROW_H
