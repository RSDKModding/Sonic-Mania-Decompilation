#ifndef OBJ_TVFLYINGBATTERY_H
#define OBJ_TVFLYINGBATTERY_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectTVFlyingBattery : Object{

};

// Entity Class
struct EntityTVFlyingBattery : Entity {

};

// Object Struct
extern ObjectTVFlyingBattery *TVFlyingBattery;

// Standard Entity Events
void TVFlyingBattery_Update();
void TVFlyingBattery_LateUpdate();
void TVFlyingBattery_StaticUpdate();
void TVFlyingBattery_Draw();
void TVFlyingBattery_Create(void* data);
void TVFlyingBattery_StageLoad();
void TVFlyingBattery_EditorDraw();
void TVFlyingBattery_EditorLoad();
void TVFlyingBattery_Serialize();

// Extra Entity Functions


#endif //!OBJ_TVFLYINGBATTERY_H
