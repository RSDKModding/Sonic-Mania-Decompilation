#ifndef OBJ_SPEEDBOOSTER_H
#define OBJ_SPEEDBOOSTER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSpeedBooster : Object{

};

// Entity Class
struct EntitySpeedBooster : Entity {

};

// Object Struct
extern ObjectSpeedBooster *SpeedBooster;

// Standard Entity Events
void SpeedBooster_Update();
void SpeedBooster_LateUpdate();
void SpeedBooster_StaticUpdate();
void SpeedBooster_Draw();
void SpeedBooster_Create(void* data);
void SpeedBooster_StageLoad();
void SpeedBooster_EditorDraw();
void SpeedBooster_EditorLoad();
void SpeedBooster_Serialize();

// Extra Entity Functions


#endif //!OBJ_SPEEDBOOSTER_H
