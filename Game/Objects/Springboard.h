#ifndef OBJ_SPRINGBOARD_H
#define OBJ_SPRINGBOARD_H

//Object Class
class Springboard : Object {

};

//Entity Class
class EntitySpringboard : Entity {

};

//Entity Functions
void Springboard_Update();
void Springboard_EarlyUpdate();
void Springboard_LateUpdate();
void Springboard_Draw();
void Springboard_Setup(void* subtype);
void Springboard_StageLoad();
void Springboard_GetAttributes();

#endif //!OBJ_SPRINGBOARD_H
