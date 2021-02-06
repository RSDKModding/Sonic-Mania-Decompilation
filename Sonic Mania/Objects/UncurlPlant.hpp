#ifndef OBJ_UNCURLPLANT_H
#define OBJ_UNCURLPLANT_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectUncurlPlant : Object {

};

// Entity Class
struct EntityUncurlPlant : Entity {

};

// Object Entity
extern ObjectUncurlPlant UncurlPlant;

// Standard Entity Events
void UncurlPlant_Update();
void UncurlPlant_LateUpdate();
void UncurlPlant_StaticUpdate();
void UncurlPlant_Draw();
void UncurlPlant_Create(void* data);
void UncurlPlant_StageLoad();
void UncurlPlant_EditorDraw();
void UncurlPlant_EditorLoad();
void UncurlPlant_Serialize();

// Extra Entity Functions


#endif //!OBJ_UNCURLPLANT_H
