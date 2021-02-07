#ifndef OBJ_UISHIFTER_H
#define OBJ_UISHIFTER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectUIShifter : Object{

};

// Entity Class
struct EntityUIShifter : Entity {

};

// Object Struct
extern ObjectUIShifter *UIShifter;

// Standard Entity Events
void UIShifter_Update();
void UIShifter_LateUpdate();
void UIShifter_StaticUpdate();
void UIShifter_Draw();
void UIShifter_Create(void* data);
void UIShifter_StageLoad();
void UIShifter_EditorDraw();
void UIShifter_EditorLoad();
void UIShifter_Serialize();

// Extra Entity Functions


#endif //!OBJ_UISHIFTER_H
