#ifndef OBJ_CABLEWARP_H
#define OBJ_CABLEWARP_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectCableWarp : Object {

};

// Entity Class
struct EntityCableWarp : Entity {

};

// Object Struct
extern ObjectCableWarp *CableWarp;

// Standard Entity Events
void CableWarp_Update();
void CableWarp_LateUpdate();
void CableWarp_StaticUpdate();
void CableWarp_Draw();
void CableWarp_Create(void* data);
void CableWarp_StageLoad();
void CableWarp_EditorDraw();
void CableWarp_EditorLoad();
void CableWarp_Serialize();

// Extra Entity Functions


#endif //!OBJ_CABLEWARP_H
