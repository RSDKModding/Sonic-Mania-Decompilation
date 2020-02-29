#ifndef OBJ_STALACTITE_H
#define OBJ_STALACTITE_H

//Object Class
class Stalactite : Object {

};

//Entity Class
class EntityStalactite : Entity {

};

//Entity Functions
void Stalactite_Update();
void Stalactite_EarlyUpdate();
void Stalactite_LateUpdate();
void Stalactite_Draw();
void Stalactite_Setup(void* subtype);
void Stalactite_StageLoad();
void Stalactite_GetAttributes();

#endif //!OBJ_STALACTITE_H
