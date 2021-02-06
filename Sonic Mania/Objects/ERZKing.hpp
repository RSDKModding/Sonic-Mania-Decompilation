#ifndef OBJ_ERZKING_H
#define OBJ_ERZKING_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectERZKing : Object {

};

// Entity Class
struct EntityERZKing : Entity {

};

// Object Entity
extern ObjectERZKing ERZKing;

// Standard Entity Events
void ERZKing_Update();
void ERZKing_LateUpdate();
void ERZKing_StaticUpdate();
void ERZKing_Draw();
void ERZKing_Create(void* data);
void ERZKing_StageLoad();
void ERZKing_EditorDraw();
void ERZKing_EditorLoad();
void ERZKing_Serialize();

// Extra Entity Functions


#endif //!OBJ_ERZKING_H
