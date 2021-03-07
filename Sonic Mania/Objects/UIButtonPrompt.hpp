#ifndef OBJ_UIBUTTONPROMPT_H
#define OBJ_UIBUTTONPROMPT_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectUIButtonPrompt : Object {

};

// Entity Class
struct EntityUIButtonPrompt : Entity {

};

// Object Struct
extern ObjectUIButtonPrompt *UIButtonPrompt;

// Standard Entity Events
void UIButtonPrompt_Update();
void UIButtonPrompt_LateUpdate();
void UIButtonPrompt_StaticUpdate();
void UIButtonPrompt_Draw();
void UIButtonPrompt_Create(void* data);
void UIButtonPrompt_StageLoad();
void UIButtonPrompt_EditorDraw();
void UIButtonPrompt_EditorLoad();
void UIButtonPrompt_Serialize();

// Extra Entity Functions


#endif //!OBJ_UIBUTTONPROMPT_H
