#ifndef OBJ_WALLBUMPER_H
#define OBJ_WALLBUMPER_H

#include "../../SonicMania.h"

//Object Class
class ObjectWallBumper : public Object {
public:

};

//Entity Class
class EntityWallBumper : public Entity {
public:

};

//Object Entity
ObjectWallBumper WallBumper;

//Entity Functions
void WallBumper_Update();
void WallBumper_EarlyUpdate();
void WallBumper_LateUpdate();
void WallBumper_Draw();
void WallBumper_Setup(void* subtype);
void WallBumper_StageLoad();
void WallBumper_GetAttributes();

#endif //!OBJ_WALLBUMPER_H
