#ifndef OBJ_PROPELLERSHAFT_H
#define OBJ_PROPELLERSHAFT_H

//Object Class
class PropellerShaft : Object {

};

//Entity Class
class EntityPropellerShaft : Entity {

};

//Entity Functions
void PropellerShaft_Update();
void PropellerShaft_EarlyUpdate();
void PropellerShaft_LateUpdate();
void PropellerShaft_Draw();
void PropellerShaft_Setup(void* subtype);
void PropellerShaft_StageLoad();
void PropellerShaft_GetAttributes();

#endif //!OBJ_PROPELLERSHAFT_H
