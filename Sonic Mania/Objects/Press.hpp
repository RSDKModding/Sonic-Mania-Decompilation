#ifndef OBJ_PRESS_H
#define OBJ_PRESS_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPress : Object {

};

// Entity Class
struct EntityPress : Entity {

};

// Object Entity
extern ObjectPress Press;

// Standard Entity Events
void Press_Update();
void Press_LateUpdate();
void Press_StaticUpdate();
void Press_Draw();
void Press_Create(void* data);
void Press_StageLoad();
void Press_EditorDraw();
void Press_EditorLoad();
void Press_Serialize();

// Extra Entity Functions


#endif //!OBJ_PRESS_H
