#ifndef OBJ_WHIRLPOOL_H
#define OBJ_WHIRLPOOL_H

//Object Class
class Whirlpool : Object {

};

//Entity Class
class EntityWhirlpool : Entity {

};

//Entity Functions
void Whirlpool_Update();
void Whirlpool_EarlyUpdate();
void Whirlpool_LateUpdate();
void Whirlpool_Draw();
void Whirlpool_Setup(void* subtype);
void Whirlpool_StageLoad();
void Whirlpool_GetAttributes();

#endif //!OBJ_WHIRLPOOL_H
