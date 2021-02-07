#ifndef OBJ_SPEEDGATE_H
#define OBJ_SPEEDGATE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSpeedGate : Object{

};

// Entity Class
struct EntitySpeedGate : Entity {

};

// Object Struct
extern ObjectSpeedGate *SpeedGate;

// Standard Entity Events
void SpeedGate_Update();
void SpeedGate_LateUpdate();
void SpeedGate_StaticUpdate();
void SpeedGate_Draw();
void SpeedGate_Create(void* data);
void SpeedGate_StageLoad();
void SpeedGate_EditorDraw();
void SpeedGate_EditorLoad();
void SpeedGate_Serialize();

// Extra Entity Functions


#endif //!OBJ_SPEEDGATE_H
