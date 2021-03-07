#ifndef OBJ_WALKERLEGS_H
#define OBJ_WALKERLEGS_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectWalkerLegs : Object {

};

// Entity Class
struct EntityWalkerLegs : Entity {

};

// Object Struct
extern ObjectWalkerLegs *WalkerLegs;

// Standard Entity Events
void WalkerLegs_Update();
void WalkerLegs_LateUpdate();
void WalkerLegs_StaticUpdate();
void WalkerLegs_Draw();
void WalkerLegs_Create(void* data);
void WalkerLegs_StageLoad();
void WalkerLegs_EditorDraw();
void WalkerLegs_EditorLoad();
void WalkerLegs_Serialize();

// Extra Entity Functions


#endif //!OBJ_WALKERLEGS_H
