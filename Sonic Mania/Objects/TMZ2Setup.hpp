#ifndef OBJ_TMZ2SETUP_H
#define OBJ_TMZ2SETUP_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectTMZ2Setup : Object {

};

// Entity Class
struct EntityTMZ2Setup : Entity {

};

// Object Struct
extern ObjectTMZ2Setup TMZ2Setup;

// Standard Entity Events
void TMZ2Setup_Update();
void TMZ2Setup_LateUpdate();
void TMZ2Setup_StaticUpdate();
void TMZ2Setup_Draw();
void TMZ2Setup_Create(void* data);
void TMZ2Setup_StageLoad();
void TMZ2Setup_EditorDraw();
void TMZ2Setup_EditorLoad();
void TMZ2Setup_Serialize();

// Extra Entity Functions


#endif //!OBJ_TMZ2SETUP_H
