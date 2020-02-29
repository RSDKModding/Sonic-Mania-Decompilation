#ifndef OBJ_CONSTELLATION_H
#define OBJ_CONSTELLATION_H

//Object Class
class Constellation : Object {

};

//Entity Class
class EntityConstellation : Entity {

};

//Entity Functions
void Constellation_Update();
void Constellation_EarlyUpdate();
void Constellation_LateUpdate();
void Constellation_Draw();
void Constellation_Setup(void* subtype);
void Constellation_StageLoad();
void Constellation_GetAttributes();

#endif //!OBJ_CONSTELLATION_H
