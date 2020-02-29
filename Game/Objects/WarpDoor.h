#ifndef OBJ_WARPDOOR_H
#define OBJ_WARPDOOR_H

//Object Class
class WarpDoor : Object {

};

//Entity Class
class EntityWarpDoor : Entity {

};

//Entity Functions
void WarpDoor_Update();
void WarpDoor_EarlyUpdate();
void WarpDoor_LateUpdate();
void WarpDoor_Draw();
void WarpDoor_Setup(void* subtype);
void WarpDoor_StageLoad();
void WarpDoor_GetAttributes();

#endif //!OBJ_WARPDOOR_H
