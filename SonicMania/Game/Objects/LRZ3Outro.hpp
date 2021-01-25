#ifndef OBJ_LRZ3OUTRO_H
#define OBJ_LRZ3OUTRO_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectLRZ3Outro : Object {

};

// Entity Class
struct EntityLRZ3Outro : Entity {

};

// Object Entity
extern ObjectLRZ3Outro LRZ3Outro;

// Standard Entity Events
void LRZ3Outro_Update();
void LRZ3Outro_LateUpdate();
void LRZ3Outro_StaticUpdate();
void LRZ3Outro_Draw();
void LRZ3Outro_Create(void* data);
void LRZ3Outro_StageLoad();
void LRZ3Outro_EditorDraw();
void LRZ3Outro_EditorLoad();
void LRZ3Outro_Serialize();

// Extra Entity Functions


#endif //!OBJ_LRZ3OUTRO_H
