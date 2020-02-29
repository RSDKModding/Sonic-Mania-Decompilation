#ifndef OBJ_TILEPLATFORM_H
#define OBJ_TILEPLATFORM_H

//Object Class
class TilePlatform : Object {

};

//Entity Class
class EntityTilePlatform : Entity {

};

//Entity Functions
void TilePlatform_Update();
void TilePlatform_EarlyUpdate();
void TilePlatform_LateUpdate();
void TilePlatform_Draw();
void TilePlatform_Setup(void* subtype);
void TilePlatform_StageLoad();
void TilePlatform_GetAttributes();

#endif //!OBJ_TILEPLATFORM_H
