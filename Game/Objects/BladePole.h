#ifndef OBJ_BLADEPOLE_H
#define OBJ_BLADEPOLE_H

//Object Class
class BladePole : Object {

};

//Entity Class
class EntityBladePole : Entity {

};

//Entity Functions
void BladePole_Update();
void BladePole_EarlyUpdate();
void BladePole_LateUpdate();
void BladePole_Draw();
void BladePole_Setup(void* subtype);
void BladePole_StageLoad();
void BladePole_GetAttributes();

#endif //!OBJ_BLADEPOLE_H
