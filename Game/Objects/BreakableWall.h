#ifndef OBJ_BREAKABLEWALL_H
#define OBJ_BREAKABLEWALL_H

//Object Class
class BreakableWall : Object {

};

//Entity Class
class EntityBreakableWall : Entity {

};

//Entity Functions
void BreakableWall_Update();
void BreakableWall_EarlyUpdate();
void BreakableWall_LateUpdate();
void BreakableWall_Draw();
void BreakableWall_Setup(void* subtype);
void BreakableWall_StageLoad();
void BreakableWall_GetAttributes();

#endif //!OBJ_BREAKABLEWALL_H
