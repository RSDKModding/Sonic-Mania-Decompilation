#ifndef OBJ_MOTOBUG_H
#define OBJ_MOTOBUG_H

//Object Class
class Motobug : Object {

};

//Entity Class
class EntityMotobug : Entity {

};

//Entity Functions
void Motobug_Update();
void Motobug_EarlyUpdate();
void Motobug_LateUpdate();
void Motobug_Draw();
void Motobug_Setup(void* subtype);
void Motobug_StageLoad();
void Motobug_GetAttributes();

#endif //!OBJ_MOTOBUG_H
