#ifndef OBJ_PIMPOM_H
#define OBJ_PIMPOM_H

//Object Class
class PimPom : Object {

};

//Entity Class
class EntityPimPom : Entity {

};

//Entity Functions
void PimPom_Update();
void PimPom_EarlyUpdate();
void PimPom_LateUpdate();
void PimPom_Draw();
void PimPom_Setup(void* subtype);
void PimPom_StageLoad();
void PimPom_GetAttributes();

#endif //!OBJ_PIMPOM_H
