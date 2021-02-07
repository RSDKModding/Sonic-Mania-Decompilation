#ifndef OBJ_LRZ1SETUP_H
#define OBJ_LRZ1SETUP_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectLRZ1Setup : Object {

};

// Entity Class
struct EntityLRZ1Setup : Entity {

};

// Object Struct
extern ObjectLRZ1Setup LRZ1Setup;

// Standard Entity Events
void LRZ1Setup_Update();
void LRZ1Setup_LateUpdate();
void LRZ1Setup_StaticUpdate();
void LRZ1Setup_Draw();
void LRZ1Setup_Create(void* data);
void LRZ1Setup_StageLoad();
void LRZ1Setup_EditorDraw();
void LRZ1Setup_EditorLoad();
void LRZ1Setup_Serialize();

// Extra Entity Functions


#endif //!OBJ_LRZ1SETUP_H
