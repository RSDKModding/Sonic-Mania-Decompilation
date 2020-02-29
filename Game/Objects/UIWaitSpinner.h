#ifndef OBJ_UIWAITSPINNER_H
#define OBJ_UIWAITSPINNER_H

#include "../../SonicMania.h"

//Object Class
class ObjectUIWaitSpinner : public Object {
public:

};

//Entity Class
class EntityUIWaitSpinner : public Entity {
public:

};

//Object Entity
ObjectUIWaitSpinner UIWaitSpinner;

//Entity Functions
void UIWaitSpinner_Update();
void UIWaitSpinner_EarlyUpdate();
void UIWaitSpinner_LateUpdate();
void UIWaitSpinner_Draw();
void UIWaitSpinner_Setup(void* subtype);
void UIWaitSpinner_StageLoad();
void UIWaitSpinner_GetAttributes();

#endif //!OBJ_UIWAITSPINNER_H
