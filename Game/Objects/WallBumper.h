#ifndef OBJ_WALLBUMPER_H
#define OBJ_WALLBUMPER_H

//Object Class
class WallBumper : Object {

};

//Entity Class
class EntityWallBumper : Entity {

};

//Entity Functions
void WallBumper_Update();
void WallBumper_EarlyUpdate();
void WallBumper_LateUpdate();
void WallBumper_Draw();
void WallBumper_Setup(void* subtype);
void WallBumper_StageLoad();
void WallBumper_GetAttributes();

#endif //!OBJ_WALLBUMPER_H
