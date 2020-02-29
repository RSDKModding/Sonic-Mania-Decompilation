#ifndef OBJ_SSZSPOTLIGHT_H
#define OBJ_SSZSPOTLIGHT_H

//Object Class
class SSZSpotlight : Object {

};

//Entity Class
class EntitySSZSpotlight : Entity {

};

//Entity Functions
void SSZSpotlight_Update();
void SSZSpotlight_EarlyUpdate();
void SSZSpotlight_LateUpdate();
void SSZSpotlight_Draw();
void SSZSpotlight_Setup(void* subtype);
void SSZSpotlight_StageLoad();
void SSZSpotlight_GetAttributes();

#endif //!OBJ_SSZSPOTLIGHT_H
