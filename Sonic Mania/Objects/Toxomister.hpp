#ifndef OBJ_TOXOMISTER_H
#define OBJ_TOXOMISTER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectToxomister : Object{

};

// Entity Class
struct EntityToxomister : Entity {

};

// Object Struct
extern ObjectToxomister *Toxomister;

// Standard Entity Events
void Toxomister_Update();
void Toxomister_LateUpdate();
void Toxomister_StaticUpdate();
void Toxomister_Draw();
void Toxomister_Create(void* data);
void Toxomister_StageLoad();
void Toxomister_EditorDraw();
void Toxomister_EditorLoad();
void Toxomister_Serialize();

// Extra Entity Functions


#endif //!OBJ_TOXOMISTER_H
