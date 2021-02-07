#ifndef OBJ_MSZSPOTLIGHT_H
#define OBJ_MSZSPOTLIGHT_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectMSZSpotlight : Object{

};

// Entity Class
struct EntityMSZSpotlight : Entity {

};

// Object Struct
extern ObjectMSZSpotlight *MSZSpotlight;

// Standard Entity Events
void MSZSpotlight_Update();
void MSZSpotlight_LateUpdate();
void MSZSpotlight_StaticUpdate();
void MSZSpotlight_Draw();
void MSZSpotlight_Create(void* data);
void MSZSpotlight_StageLoad();
void MSZSpotlight_EditorDraw();
void MSZSpotlight_EditorLoad();
void MSZSpotlight_Serialize();

// Extra Entity Functions


#endif //!OBJ_MSZSPOTLIGHT_H
