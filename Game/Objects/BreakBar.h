#ifndef OBJ_BREAKBAR_H
#define OBJ_BREAKBAR_H

#include "../../SonicMania.h"

//Object Class
class ObjectBreakBar : public Object {
public:

};

//Entity Class
class EntityBreakBar : public Entity {
public:

};

//Object Entity
ObjectBreakBar BreakBar;

//Entity Functions
void BreakBar_Update();
void BreakBar_EarlyUpdate();
void BreakBar_LateUpdate();
void BreakBar_Draw();
void BreakBar_Setup(void* subtype);
void BreakBar_StageLoad();
void BreakBar_GetAttributes();

#endif //!OBJ_BREAKBAR_H
