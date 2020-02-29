#ifndef OBJ_TWISTINGDOOR_H
#define OBJ_TWISTINGDOOR_H

//Object Class
class TwistingDoor : Object {

};

//Entity Class
class EntityTwistingDoor : Entity {

};

//Entity Functions
void TwistingDoor_Update();
void TwistingDoor_EarlyUpdate();
void TwistingDoor_LateUpdate();
void TwistingDoor_Draw();
void TwistingDoor_Setup(void* subtype);
void TwistingDoor_StageLoad();
void TwistingDoor_GetAttributes();

#endif //!OBJ_TWISTINGDOOR_H
