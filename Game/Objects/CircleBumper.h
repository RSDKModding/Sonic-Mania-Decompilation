#ifndef OBJ_CIRCLEBUMPER_H
#define OBJ_CIRCLEBUMPER_H

//Object Class
class CircleBumper : Object {

};

//Entity Class
class EntityCircleBumper : Entity {

};

//Entity Functions
void CircleBumper_Update();
void CircleBumper_EarlyUpdate();
void CircleBumper_LateUpdate();
void CircleBumper_Draw();
void CircleBumper_Setup(void* subtype);
void CircleBumper_StageLoad();
void CircleBumper_GetAttributes();

#endif //!OBJ_CIRCLEBUMPER_H
