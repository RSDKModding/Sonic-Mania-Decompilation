#ifndef OBJ_SPINBOOSTER_H
#define OBJ_SPINBOOSTER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectSpinBooster : Object{

};

// Entity Class
struct EntitySpinBooster : Entity {

};

// Object Struct
extern ObjectSpinBooster *SpinBooster;

// Standard Entity Events
void SpinBooster_Update();
void SpinBooster_LateUpdate();
void SpinBooster_StaticUpdate();
void SpinBooster_Draw();
void SpinBooster_Create(void* data);
void SpinBooster_StageLoad();
void SpinBooster_EditorDraw();
void SpinBooster_EditorLoad();
void SpinBooster_Serialize();

// Extra Entity Functions


#endif //!OBJ_SPINBOOSTER_H
