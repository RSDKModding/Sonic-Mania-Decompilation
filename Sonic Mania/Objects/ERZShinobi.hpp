#ifndef OBJ_ERZSHINOBI_H
#define OBJ_ERZSHINOBI_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectERZShinobi : Object{

};

// Entity Class
struct EntityERZShinobi : Entity {

};

// Object Struct
extern ObjectERZShinobi *ERZShinobi;

// Standard Entity Events
void ERZShinobi_Update();
void ERZShinobi_LateUpdate();
void ERZShinobi_StaticUpdate();
void ERZShinobi_Draw();
void ERZShinobi_Create(void* data);
void ERZShinobi_StageLoad();
void ERZShinobi_EditorDraw();
void ERZShinobi_EditorLoad();
void ERZShinobi_Serialize();

// Extra Entity Functions


#endif //!OBJ_ERZSHINOBI_H
