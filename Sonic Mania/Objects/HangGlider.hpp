#ifndef OBJ_HANGGLIDER_H
#define OBJ_HANGGLIDER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectHangGlider : Object {

};

// Entity Class
struct EntityHangGlider : Entity {

};

// Object Struct
extern ObjectHangGlider *HangGlider;

// Standard Entity Events
void HangGlider_Update();
void HangGlider_LateUpdate();
void HangGlider_StaticUpdate();
void HangGlider_Draw();
void HangGlider_Create(void* data);
void HangGlider_StageLoad();
void HangGlider_EditorDraw();
void HangGlider_EditorLoad();
void HangGlider_Serialize();

// Extra Entity Functions


#endif //!OBJ_HANGGLIDER_H
