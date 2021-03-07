#ifndef OBJ_ERZMYSTIC_H
#define OBJ_ERZMYSTIC_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectERZMystic : Object {

};

// Entity Class
struct EntityERZMystic : Entity {

};

// Object Struct
extern ObjectERZMystic *ERZMystic;

// Standard Entity Events
void ERZMystic_Update();
void ERZMystic_LateUpdate();
void ERZMystic_StaticUpdate();
void ERZMystic_Draw();
void ERZMystic_Create(void* data);
void ERZMystic_StageLoad();
void ERZMystic_EditorDraw();
void ERZMystic_EditorLoad();
void ERZMystic_Serialize();

// Extra Entity Functions


#endif //!OBJ_ERZMYSTIC_H
