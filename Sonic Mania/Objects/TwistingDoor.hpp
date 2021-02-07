#ifndef OBJ_TWISTINGDOOR_H
#define OBJ_TWISTINGDOOR_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectTwistingDoor : Object{

};

// Entity Class
struct EntityTwistingDoor : Entity {

};

// Object Struct
extern ObjectTwistingDoor *TwistingDoor;

// Standard Entity Events
void TwistingDoor_Update();
void TwistingDoor_LateUpdate();
void TwistingDoor_StaticUpdate();
void TwistingDoor_Draw();
void TwistingDoor_Create(void* data);
void TwistingDoor_StageLoad();
void TwistingDoor_EditorDraw();
void TwistingDoor_EditorLoad();
void TwistingDoor_Serialize();

// Extra Entity Functions


#endif //!OBJ_TWISTINGDOOR_H
