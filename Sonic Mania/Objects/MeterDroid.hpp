#ifndef OBJ_METERDROID_H
#define OBJ_METERDROID_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectMeterDroid : Object{

};

// Entity Class
struct EntityMeterDroid : Entity {

};

// Object Struct
extern ObjectMeterDroid *MeterDroid;

// Standard Entity Events
void MeterDroid_Update();
void MeterDroid_LateUpdate();
void MeterDroid_StaticUpdate();
void MeterDroid_Draw();
void MeterDroid_Create(void* data);
void MeterDroid_StageLoad();
void MeterDroid_EditorDraw();
void MeterDroid_EditorLoad();
void MeterDroid_Serialize();

// Extra Entity Functions


#endif //!OBJ_METERDROID_H
