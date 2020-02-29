#ifndef OBJ_GIANTPISTOL_H
#define OBJ_GIANTPISTOL_H

//Object Class
class GiantPistol : Object {

};

//Entity Class
class EntityGiantPistol : Entity {

};

//Entity Functions
void GiantPistol_Update();
void GiantPistol_EarlyUpdate();
void GiantPistol_LateUpdate();
void GiantPistol_Draw();
void GiantPistol_Setup(void* subtype);
void GiantPistol_StageLoad();
void GiantPistol_GetAttributes();

#endif //!OBJ_GIANTPISTOL_H
