#ifndef OBJ_BUTTONDOOR_H
#define OBJ_BUTTONDOOR_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectButtonDoor : Object {

};

// Entity Class
struct EntityButtonDoor : Entity {

};

// Object Entity
extern ObjectButtonDoor ButtonDoor;

// Standard Entity Events
void ButtonDoor_Update();
void ButtonDoor_LateUpdate();
void ButtonDoor_StaticUpdate();
void ButtonDoor_Draw();
void ButtonDoor_Create(void* data);
void ButtonDoor_StageLoad();
void ButtonDoor_EditorDraw();
void ButtonDoor_EditorLoad();
void ButtonDoor_Serialize();

// Extra Entity Functions


#endif //!OBJ_BUTTONDOOR_H
