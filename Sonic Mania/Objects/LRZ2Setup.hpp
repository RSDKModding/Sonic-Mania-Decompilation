#ifndef OBJ_LRZ2SETUP_H
#define OBJ_LRZ2SETUP_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectLRZ2Setup : Object {

};

// Entity Class
struct EntityLRZ2Setup : Entity {

};

// Object Entity
extern ObjectLRZ2Setup LRZ2Setup;

// Standard Entity Events
void LRZ2Setup_Update();
void LRZ2Setup_LateUpdate();
void LRZ2Setup_StaticUpdate();
void LRZ2Setup_Draw();
void LRZ2Setup_Create(void* data);
void LRZ2Setup_StageLoad();
void LRZ2Setup_EditorDraw();
void LRZ2Setup_EditorLoad();
void LRZ2Setup_Serialize();

// Extra Entity Functions


#endif //!OBJ_LRZ2SETUP_H
