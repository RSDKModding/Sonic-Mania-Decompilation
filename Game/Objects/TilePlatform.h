#ifndef OBJ_TILEPLATFORM_H
#define OBJ_TILEPLATFORM_H

#include "../../SonicMania.h"

//Object Class
class ObjectTilePlatform : public Object {
public:

};

//Entity Class
class EntityTilePlatform : public Entity {
public:

};

//Object Entity
ObjectTilePlatform TilePlatform;

//Entity Functions
void TilePlatform_Update();
void TilePlatform_EarlyUpdate();
void TilePlatform_LateUpdate();
void TilePlatform_Draw();
void TilePlatform_Setup(void* subtype);
void TilePlatform_StageLoad();
void TilePlatform_GetAttributes();

#endif //!OBJ_TILEPLATFORM_H
