#ifndef OBJ_HANGCONVEYOR_H
#define OBJ_HANGCONVEYOR_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectHangConveyor : Object {

};

// Entity Class
struct EntityHangConveyor : Entity {

};

// Object Struct
extern ObjectHangConveyor *HangConveyor;

// Standard Entity Events
void HangConveyor_Update();
void HangConveyor_LateUpdate();
void HangConveyor_StaticUpdate();
void HangConveyor_Draw();
void HangConveyor_Create(void* data);
void HangConveyor_StageLoad();
void HangConveyor_EditorDraw();
void HangConveyor_EditorLoad();
void HangConveyor_Serialize();

// Extra Entity Functions


#endif //!OBJ_HANGCONVEYOR_H
