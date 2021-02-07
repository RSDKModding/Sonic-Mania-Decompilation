#ifndef OBJ_LRZ1OUTRO_H
#define OBJ_LRZ1OUTRO_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectLRZ1Outro : Object {

};

// Entity Class
struct EntityLRZ1Outro : Entity {

};

// Object Struct
extern ObjectLRZ1Outro LRZ1Outro;

// Standard Entity Events
void LRZ1Outro_Update();
void LRZ1Outro_LateUpdate();
void LRZ1Outro_StaticUpdate();
void LRZ1Outro_Draw();
void LRZ1Outro_Create(void* data);
void LRZ1Outro_StageLoad();
void LRZ1Outro_EditorDraw();
void LRZ1Outro_EditorLoad();
void LRZ1Outro_Serialize();

// Extra Entity Functions


#endif //!OBJ_LRZ1OUTRO_H
