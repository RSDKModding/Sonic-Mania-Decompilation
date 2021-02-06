#ifndef OBJ_LRZ3OUTROK_H
#define OBJ_LRZ3OUTROK_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectLRZ3OutroK : Object {

};

// Entity Class
struct EntityLRZ3OutroK : Entity {

};

// Object Entity
extern ObjectLRZ3OutroK LRZ3OutroK;

// Standard Entity Events
void LRZ3OutroK_Update();
void LRZ3OutroK_LateUpdate();
void LRZ3OutroK_StaticUpdate();
void LRZ3OutroK_Draw();
void LRZ3OutroK_Create(void* data);
void LRZ3OutroK_StageLoad();
void LRZ3OutroK_EditorDraw();
void LRZ3OutroK_EditorLoad();
void LRZ3OutroK_Serialize();

// Extra Entity Functions


#endif //!OBJ_LRZ3OUTROK_H
