#ifndef OBJ_LRZCONVITEM_H
#define OBJ_LRZCONVITEM_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectLRZConvItem : Object {

};

// Entity Class
struct EntityLRZConvItem : Entity {

};

// Object Struct
extern ObjectLRZConvItem *LRZConvItem;

// Standard Entity Events
void LRZConvItem_Update();
void LRZConvItem_LateUpdate();
void LRZConvItem_StaticUpdate();
void LRZConvItem_Draw();
void LRZConvItem_Create(void* data);
void LRZConvItem_StageLoad();
void LRZConvItem_EditorDraw();
void LRZConvItem_EditorLoad();
void LRZConvItem_Serialize();

// Extra Entity Functions


#endif //!OBJ_LRZCONVITEM_H
