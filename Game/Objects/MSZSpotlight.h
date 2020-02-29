#ifndef OBJ_MSZSPOTLIGHT_H
#define OBJ_MSZSPOTLIGHT_H

//Object Class
class MSZSpotlight : Object {

};

//Entity Class
class EntityMSZSpotlight : Entity {

};

//Entity Functions
void MSZSpotlight_Update();
void MSZSpotlight_EarlyUpdate();
void MSZSpotlight_LateUpdate();
void MSZSpotlight_Draw();
void MSZSpotlight_Setup(void* subtype);
void MSZSpotlight_StageLoad();
void MSZSpotlight_GetAttributes();

#endif //!OBJ_MSZSPOTLIGHT_H
