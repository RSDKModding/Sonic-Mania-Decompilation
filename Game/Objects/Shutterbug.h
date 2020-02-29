#ifndef OBJ_SHUTTERBUG_H
#define OBJ_SHUTTERBUG_H

//Object Class
class Shutterbug : Object {

};

//Entity Class
class EntityShutterbug : Entity {

};

//Entity Functions
void Shutterbug_Update();
void Shutterbug_EarlyUpdate();
void Shutterbug_LateUpdate();
void Shutterbug_Draw();
void Shutterbug_Setup(void* subtype);
void Shutterbug_StageLoad();
void Shutterbug_GetAttributes();

#endif //!OBJ_SHUTTERBUG_H
