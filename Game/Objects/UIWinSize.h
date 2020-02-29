#ifndef OBJ_UIWINSIZE_H
#define OBJ_UIWINSIZE_H

#include "../../SonicMania.h"

//Object Class
class ObjectUIWinSize : public Object {
public:

};

//Entity Class
class EntityUIWinSize : public Entity {
public:

};

//Object Entity
ObjectUIWinSize UIWinSize;

//Entity Functions
void UIWinSize_Update();
void UIWinSize_EarlyUpdate();
void UIWinSize_LateUpdate();
void UIWinSize_Draw();
void UIWinSize_Setup(void* subtype);
void UIWinSize_StageLoad();
void UIWinSize_GetAttributes();

#endif //!OBJ_UIWINSIZE_H
