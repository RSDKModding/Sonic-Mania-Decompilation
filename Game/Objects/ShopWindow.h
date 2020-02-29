#ifndef OBJ_SHOPWINDOW_H
#define OBJ_SHOPWINDOW_H

#include "../../SonicMania.h"

//Object Class
class ObjectShopWindow : public Object {
public:

};

//Entity Class
class EntityShopWindow : public Entity {
public:

};

//Object Entity
ObjectShopWindow ShopWindow;

//Entity Functions
void ShopWindow_Update();
void ShopWindow_EarlyUpdate();
void ShopWindow_LateUpdate();
void ShopWindow_Draw();
void ShopWindow_Setup(void* subtype);
void ShopWindow_StageLoad();
void ShopWindow_GetAttributes();

#endif //!OBJ_SHOPWINDOW_H
