#ifndef OBJ_HOTARUHIWATT_H
#define OBJ_HOTARUHIWATT_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectHotaruHiWatt : Object{

};

// Entity Class
struct EntityHotaruHiWatt : Entity {

};

// Object Struct
extern ObjectHotaruHiWatt *HotaruHiWatt;

// Standard Entity Events
void HotaruHiWatt_Update();
void HotaruHiWatt_LateUpdate();
void HotaruHiWatt_StaticUpdate();
void HotaruHiWatt_Draw();
void HotaruHiWatt_Create(void* data);
void HotaruHiWatt_StageLoad();
void HotaruHiWatt_EditorDraw();
void HotaruHiWatt_EditorLoad();
void HotaruHiWatt_Serialize();

// Extra Entity Functions


#endif //!OBJ_HOTARUHIWATT_H
