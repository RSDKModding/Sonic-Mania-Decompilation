#ifndef OBJ_LRZ3SETUP_H
#define OBJ_LRZ3SETUP_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectLRZ3Setup : Object {

};

// Entity Class
struct EntityLRZ3Setup : Entity {

};

// Object Struct
extern ObjectLRZ3Setup LRZ3Setup;

// Standard Entity Events
void LRZ3Setup_Update();
void LRZ3Setup_LateUpdate();
void LRZ3Setup_StaticUpdate();
void LRZ3Setup_Draw();
void LRZ3Setup_Create(void* data);
void LRZ3Setup_StageLoad();
void LRZ3Setup_EditorDraw();
void LRZ3Setup_EditorLoad();
void LRZ3Setup_Serialize();

// Extra Entity Functions


#endif //!OBJ_LRZ3SETUP_H
