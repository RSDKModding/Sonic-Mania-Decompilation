#ifndef OBJ_REAGENT_H
#define OBJ_REAGENT_H

//Object Class
class Reagent : Object {

};

//Entity Class
class EntityReagent : Entity {

};

//Entity Functions
void Reagent_Update();
void Reagent_EarlyUpdate();
void Reagent_LateUpdate();
void Reagent_Draw();
void Reagent_Setup(void* subtype);
void Reagent_StageLoad();
void Reagent_GetAttributes();

#endif //!OBJ_REAGENT_H
