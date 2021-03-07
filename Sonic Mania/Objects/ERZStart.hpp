#ifndef OBJ_ERZSTART_H
#define OBJ_ERZSTART_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectERZStart : Object {

};

// Entity Class
struct EntityERZStart : Entity {

};

// Object Struct
extern ObjectERZStart *ERZStart;

// Standard Entity Events
void ERZStart_Update();
void ERZStart_LateUpdate();
void ERZStart_StaticUpdate();
void ERZStart_Draw();
void ERZStart_Create(void* data);
void ERZStart_StageLoad();
void ERZStart_EditorDraw();
void ERZStart_EditorLoad();
void ERZStart_Serialize();

// Extra Entity Functions


#endif //!OBJ_ERZSTART_H
