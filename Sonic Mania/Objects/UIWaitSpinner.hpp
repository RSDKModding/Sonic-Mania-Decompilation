#ifndef OBJ_UIWAITSPINNER_H
#define OBJ_UIWAITSPINNER_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectUIWaitSpinner : Object {

};

// Entity Class
struct EntityUIWaitSpinner : Entity {

};

// Object Struct
extern ObjectUIWaitSpinner *UIWaitSpinner;

// Standard Entity Events
void UIWaitSpinner_Update();
void UIWaitSpinner_LateUpdate();
void UIWaitSpinner_StaticUpdate();
void UIWaitSpinner_Draw();
void UIWaitSpinner_Create(void* data);
void UIWaitSpinner_StageLoad();
void UIWaitSpinner_EditorDraw();
void UIWaitSpinner_EditorLoad();
void UIWaitSpinner_Serialize();

// Extra Entity Functions


#endif //!OBJ_UIWAITSPINNER_H
