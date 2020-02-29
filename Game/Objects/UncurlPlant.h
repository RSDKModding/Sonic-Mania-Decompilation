#ifndef OBJ_UNCURLPLANT_H
#define OBJ_UNCURLPLANT_H

//Object Class
class UncurlPlant : Object {

};

//Entity Class
class EntityUncurlPlant : Entity {

};

//Entity Functions
void UncurlPlant_Update();
void UncurlPlant_EarlyUpdate();
void UncurlPlant_LateUpdate();
void UncurlPlant_Draw();
void UncurlPlant_Setup(void* subtype);
void UncurlPlant_StageLoad();
void UncurlPlant_GetAttributes();

#endif //!OBJ_UNCURLPLANT_H
