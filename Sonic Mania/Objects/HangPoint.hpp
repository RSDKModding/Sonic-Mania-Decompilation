#ifndef OBJ_HANGPOINT_H
#define OBJ_HANGPOINT_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectHangPoint : Object{

};

// Entity Class
struct EntityHangPoint : Entity {

};

// Object Struct
extern ObjectHangPoint *HangPoint;

// Standard Entity Events
void HangPoint_Update();
void HangPoint_LateUpdate();
void HangPoint_StaticUpdate();
void HangPoint_Draw();
void HangPoint_Create(void* data);
void HangPoint_StageLoad();
void HangPoint_EditorDraw();
void HangPoint_EditorLoad();
void HangPoint_Serialize();

// Extra Entity Functions


#endif //!OBJ_HANGPOINT_H
