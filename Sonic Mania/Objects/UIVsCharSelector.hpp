#ifndef OBJ_UIVSCHARSELECTOR_H
#define OBJ_UIVSCHARSELECTOR_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectUIVsCharSelector : Object {

};

// Entity Class
struct EntityUIVsCharSelector : Entity {

};

// Object Struct
extern ObjectUIVsCharSelector *UIVsCharSelector;

// Standard Entity Events
void UIVsCharSelector_Update();
void UIVsCharSelector_LateUpdate();
void UIVsCharSelector_StaticUpdate();
void UIVsCharSelector_Draw();
void UIVsCharSelector_Create(void* data);
void UIVsCharSelector_StageLoad();
void UIVsCharSelector_EditorDraw();
void UIVsCharSelector_EditorLoad();
void UIVsCharSelector_Serialize();

// Extra Entity Functions


#endif //!OBJ_UIVSCHARSELECTOR_H
