#ifndef OBJ_REAGENT_H
#define OBJ_REAGENT_H

#include "../../SonicMania.h"

//Object Class
class ObjectReagent : public Object {
public:

};

//Entity Class
class EntityReagent : public Entity {
public:

};

//Object Entity
ObjectReagent Reagent;

//Entity Functions
void Reagent_Update();
void Reagent_EarlyUpdate();
void Reagent_LateUpdate();
void Reagent_Draw();
void Reagent_Setup(void* subtype);
void Reagent_StageLoad();
void Reagent_GetAttributes();

#endif //!OBJ_REAGENT_H
