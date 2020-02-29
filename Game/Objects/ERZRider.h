#ifndef OBJ_ERZRIDER_H
#define OBJ_ERZRIDER_H

//Object Class
class ERZRider : Object {

};

//Entity Class
class EntityERZRider : Entity {

};

//Entity Functions
void ERZRider_Update();
void ERZRider_EarlyUpdate();
void ERZRider_LateUpdate();
void ERZRider_Draw();
void ERZRider_Setup(void* subtype);
void ERZRider_StageLoad();
void ERZRider_GetAttributes();

#endif //!OBJ_ERZRIDER_H
