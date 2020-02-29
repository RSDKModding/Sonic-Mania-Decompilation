#ifndef OBJ_ICE_H
#define OBJ_ICE_H

//Object Class
class Ice : Object {

};

//Entity Class
class EntityIce : Entity {

};

//Entity Functions
void Ice_Update();
void Ice_EarlyUpdate();
void Ice_LateUpdate();
void Ice_Draw();
void Ice_Setup(void* subtype);
void Ice_StageLoad();
void Ice_GetAttributes();

#endif //!OBJ_ICE_H
