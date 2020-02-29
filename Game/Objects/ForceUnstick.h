#ifndef OBJ_FORCEUNSTICK_H
#define OBJ_FORCEUNSTICK_H

//Object Class
class ForceUnstick : Object {

};

//Entity Class
class EntityForceUnstick : Entity {

};

//Entity Functions
void ForceUnstick_Update();
void ForceUnstick_EarlyUpdate();
void ForceUnstick_LateUpdate();
void ForceUnstick_Draw();
void ForceUnstick_Setup(void* subtype);
void ForceUnstick_StageLoad();
void ForceUnstick_GetAttributes();

#endif //!OBJ_FORCEUNSTICK_H
