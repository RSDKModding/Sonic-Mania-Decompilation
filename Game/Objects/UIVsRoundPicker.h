#ifndef OBJ_UIVSROUNDPICKER_H
#define OBJ_UIVSROUNDPICKER_H

#include "../../SonicMania.h"

//Object Class
class ObjectUIVsRoundPicker : public Object {
public:

};

//Entity Class
class EntityUIVsRoundPicker : public Entity {
public:

};

//Object Entity
ObjectUIVsRoundPicker UIVsRoundPicker;

//Entity Functions
void UIVsRoundPicker_Update();
void UIVsRoundPicker_EarlyUpdate();
void UIVsRoundPicker_LateUpdate();
void UIVsRoundPicker_Draw();
void UIVsRoundPicker_Setup(void* subtype);
void UIVsRoundPicker_StageLoad();
void UIVsRoundPicker_GetAttributes();

#endif //!OBJ_UIVSROUNDPICKER_H
