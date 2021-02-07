#ifndef OBJ_HEAVYGUNNER_H
#define OBJ_HEAVYGUNNER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectHeavyGunner : Object{

};

// Entity Class
struct EntityHeavyGunner : Entity {

};

// Object Struct
extern ObjectHeavyGunner *HeavyGunner;

// Standard Entity Events
void HeavyGunner_Update();
void HeavyGunner_LateUpdate();
void HeavyGunner_StaticUpdate();
void HeavyGunner_Draw();
void HeavyGunner_Create(void* data);
void HeavyGunner_StageLoad();
void HeavyGunner_EditorDraw();
void HeavyGunner_EditorLoad();
void HeavyGunner_Serialize();

// Extra Entity Functions


#endif //!OBJ_HEAVYGUNNER_H
