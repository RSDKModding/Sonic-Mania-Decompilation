#ifndef OBJ_DRAGONFLY_H
#define OBJ_DRAGONFLY_H

//Object Class
class Dragonfly : Object {

};

//Entity Class
class EntityDragonfly : Entity {

};

//Entity Functions
void Dragonfly_Update();
void Dragonfly_EarlyUpdate();
void Dragonfly_LateUpdate();
void Dragonfly_Draw();
void Dragonfly_Setup(void* subtype);
void Dragonfly_StageLoad();
void Dragonfly_GetAttributes();

#endif //!OBJ_DRAGONFLY_H
