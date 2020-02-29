#ifndef OBJ_BREAKBAR_H
#define OBJ_BREAKBAR_H

//Object Class
class BreakBar : Object {

};

//Entity Class
class EntityBreakBar : Entity {

};

//Entity Functions
void BreakBar_Update();
void BreakBar_EarlyUpdate();
void BreakBar_LateUpdate();
void BreakBar_Draw();
void BreakBar_Setup(void* subtype);
void BreakBar_StageLoad();
void BreakBar_GetAttributes();

#endif //!OBJ_BREAKBAR_H
