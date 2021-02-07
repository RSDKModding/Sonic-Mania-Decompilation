#ifndef OBJ_UIBACKGROUND_H
#define OBJ_UIBACKGROUND_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectUIBackground : Object{

};

// Entity Class
struct EntityUIBackground : Entity {

};

// Object Struct
extern ObjectUIBackground *UIBackground;

// Standard Entity Events
void UIBackground_Update();
void UIBackground_LateUpdate();
void UIBackground_StaticUpdate();
void UIBackground_Draw();
void UIBackground_Create(void* data);
void UIBackground_StageLoad();
void UIBackground_EditorDraw();
void UIBackground_EditorLoad();
void UIBackground_Serialize();

// Extra Entity Functions


#endif //!OBJ_UIBACKGROUND_H
