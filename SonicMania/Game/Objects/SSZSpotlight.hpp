#ifndef OBJ_SSZSPOTLIGHT_H
#define OBJ_SSZSPOTLIGHT_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSSZSpotlight : Object {

};

// Entity Class
struct EntitySSZSpotlight : Entity {

};

// Object Entity
extern ObjectSSZSpotlight SSZSpotlight;

// Standard Entity Events
void SSZSpotlight_Update();
void SSZSpotlight_LateUpdate();
void SSZSpotlight_StaticUpdate();
void SSZSpotlight_Draw();
void SSZSpotlight_Create(void* data);
void SSZSpotlight_StageLoad();
void SSZSpotlight_EditorDraw();
void SSZSpotlight_EditorLoad();
void SSZSpotlight_Serialize();

// Extra Entity Functions


#endif //!OBJ_SSZSPOTLIGHT_H
