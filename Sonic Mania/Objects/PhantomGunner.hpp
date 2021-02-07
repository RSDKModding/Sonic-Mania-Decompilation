#ifndef OBJ_PHANTOMGUNNER_H
#define OBJ_PHANTOMGUNNER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectPhantomGunner : Object{

};

// Entity Class
struct EntityPhantomGunner : Entity {

};

// Object Struct
extern ObjectPhantomGunner *PhantomGunner;

// Standard Entity Events
void PhantomGunner_Update();
void PhantomGunner_LateUpdate();
void PhantomGunner_StaticUpdate();
void PhantomGunner_Draw();
void PhantomGunner_Create(void* data);
void PhantomGunner_StageLoad();
void PhantomGunner_EditorDraw();
void PhantomGunner_EditorLoad();
void PhantomGunner_Serialize();

// Extra Entity Functions


#endif //!OBJ_PHANTOMGUNNER_H
