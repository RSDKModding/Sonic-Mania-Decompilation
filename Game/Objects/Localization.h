#ifndef OBJ_LOCALIZATION_H
#define OBJ_LOCALIZATION_H

#include "../../SonicMania.h"

//Object Class
class ObjectLocalization : public Object {
public:

};

//Entity Class
class EntityLocalization : public Entity {
public:

};

//Object Entity
ObjectLocalization Localization;

//Entity Functions
void Localization_Update();
void Localization_EarlyUpdate();
void Localization_LateUpdate();
void Localization_Draw();
void Localization_Setup(void* subtype);
void Localization_StageLoad();
void Localization_GetAttributes();

#endif //!OBJ_LOCALIZATION_H
