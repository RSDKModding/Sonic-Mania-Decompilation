#ifndef OBJ_LRZCONVEYOR_H
#define OBJ_LRZCONVEYOR_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectLRZConveyor : Object {

};

// Entity Class
struct EntityLRZConveyor : Entity {

};

// Object Entity
extern ObjectLRZConveyor LRZConveyor;

// Standard Entity Events
void LRZConveyor_Update();
void LRZConveyor_LateUpdate();
void LRZConveyor_StaticUpdate();
void LRZConveyor_Draw();
void LRZConveyor_Create(void* data);
void LRZConveyor_StageLoad();
void LRZConveyor_EditorDraw();
void LRZConveyor_EditorLoad();
void LRZConveyor_Serialize();

// Extra Entity Functions


#endif //!OBJ_LRZCONVEYOR_H
