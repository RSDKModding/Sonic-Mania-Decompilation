#ifndef OBJ_UIWINSIZE_H
#define OBJ_UIWINSIZE_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectUIWinSize : Object{

};

// Entity Class
struct EntityUIWinSize : Entity {

};

// Object Struct
extern ObjectUIWinSize *UIWinSize;

// Standard Entity Events
void UIWinSize_Update();
void UIWinSize_LateUpdate();
void UIWinSize_StaticUpdate();
void UIWinSize_Draw();
void UIWinSize_Create(void* data);
void UIWinSize_StageLoad();
void UIWinSize_EditorDraw();
void UIWinSize_EditorLoad();
void UIWinSize_Serialize();

// Extra Entity Functions


#endif //!OBJ_UIWINSIZE_H
