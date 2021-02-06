#ifndef OBJ_CURRENT_H
#define OBJ_CURRENT_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectCurrent : Object {

};

// Entity Class
struct EntityCurrent : Entity {

};

// Object Entity
extern ObjectCurrent Current;

// Standard Entity Events
void Current_Update();
void Current_LateUpdate();
void Current_StaticUpdate();
void Current_Draw();
void Current_Create(void* data);
void Current_StageLoad();
void Current_EditorDraw();
void Current_EditorLoad();
void Current_Serialize();

// Extra Entity Functions


#endif //!OBJ_CURRENT_H
