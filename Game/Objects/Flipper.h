#ifndef OBJ_FLIPPER_H
#define OBJ_FLIPPER_H

//Object Class
class Flipper : Object {

};

//Entity Class
class EntityFlipper : Entity {

};

//Entity Functions
void Flipper_Update();
void Flipper_EarlyUpdate();
void Flipper_LateUpdate();
void Flipper_Draw();
void Flipper_Setup(void* subtype);
void Flipper_StageLoad();
void Flipper_GetAttributes();

#endif //!OBJ_FLIPPER_H
