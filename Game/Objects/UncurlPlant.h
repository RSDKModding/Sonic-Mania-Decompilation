#ifndef OBJ_UNCURLPLANT_H
#define OBJ_UNCURLPLANT_H

#include "../../SonicMania.h"

//Object Class
class ObjectUncurlPlant : public Object {
public:

};

//Entity Class
class EntityUncurlPlant : public Entity {
public:

};

//Object Entity
ObjectUncurlPlant UncurlPlant;

//Entity Functions
void UncurlPlant_Update();
void UncurlPlant_EarlyUpdate();
void UncurlPlant_LateUpdate();
void UncurlPlant_Draw();
void UncurlPlant_Setup(void* subtype);
void UncurlPlant_StageLoad();
void UncurlPlant_GetAttributes();

#endif //!OBJ_UNCURLPLANT_H
