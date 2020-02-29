#ifndef OBJ_LRZCONVITEM_H
#define OBJ_LRZCONVITEM_H

//Object Class
class LRZConvItem : Object {

};

//Entity Class
class EntityLRZConvItem : Entity {

};

//Entity Functions
void LRZConvItem_Update();
void LRZConvItem_EarlyUpdate();
void LRZConvItem_LateUpdate();
void LRZConvItem_Draw();
void LRZConvItem_Setup(void* subtype);
void LRZConvItem_StageLoad();
void LRZConvItem_GetAttributes();

#endif //!OBJ_LRZCONVITEM_H
