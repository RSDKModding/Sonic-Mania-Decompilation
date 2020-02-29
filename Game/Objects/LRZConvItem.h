#ifndef OBJ_LRZCONVITEM_H
#define OBJ_LRZCONVITEM_H

#include "../../SonicMania.h"

//Object Class
class ObjectLRZConvItem : public Object {
public:

};

//Entity Class
class EntityLRZConvItem : public Entity {
public:

};

//Object Entity
ObjectLRZConvItem LRZConvItem;

//Entity Functions
void LRZConvItem_Update();
void LRZConvItem_EarlyUpdate();
void LRZConvItem_LateUpdate();
void LRZConvItem_Draw();
void LRZConvItem_Setup(void* subtype);
void LRZConvItem_StageLoad();
void LRZConvItem_GetAttributes();

#endif //!OBJ_LRZCONVITEM_H
