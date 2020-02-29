#ifndef OBJ_MMZLIGHTNING_H
#define OBJ_MMZLIGHTNING_H

#include "../../SonicMania.h"

//Object Class
class ObjectMMZLightning : public Object {
public:

};

//Entity Class
class EntityMMZLightning : public Entity {
public:

};

//Object Entity
ObjectMMZLightning MMZLightning;

//Entity Functions
void MMZLightning_Update();
void MMZLightning_EarlyUpdate();
void MMZLightning_LateUpdate();
void MMZLightning_Draw();
void MMZLightning_Setup(void* subtype);
void MMZLightning_StageLoad();
void MMZLightning_GetAttributes();

#endif //!OBJ_MMZLIGHTNING_H
