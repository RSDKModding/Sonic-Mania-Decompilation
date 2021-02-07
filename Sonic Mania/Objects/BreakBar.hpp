#ifndef OBJ_BREAKBAR_H
#define OBJ_BREAKBAR_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectBreakBar : Object{

};

// Entity Class
struct EntityBreakBar : Entity {

};

// Object Struct
extern ObjectBreakBar *BreakBar;

// Standard Entity Events
void BreakBar_Update();
void BreakBar_LateUpdate();
void BreakBar_StaticUpdate();
void BreakBar_Draw();
void BreakBar_Create(void* data);
void BreakBar_StageLoad();
void BreakBar_EditorDraw();
void BreakBar_EditorLoad();
void BreakBar_Serialize();

// Extra Entity Functions


#endif //!OBJ_BREAKBAR_H
