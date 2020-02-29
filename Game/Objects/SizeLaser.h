#ifndef OBJ_SIZELASER_H
#define OBJ_SIZELASER_H

//Object Class
class SizeLaser : Object {

};

//Entity Class
class EntitySizeLaser : Entity {

};

//Entity Functions
void SizeLaser_Update();
void SizeLaser_EarlyUpdate();
void SizeLaser_LateUpdate();
void SizeLaser_Draw();
void SizeLaser_Setup(void* subtype);
void SizeLaser_StageLoad();
void SizeLaser_GetAttributes();

#endif //!OBJ_SIZELASER_H
