#ifndef OBJ_SHOPWINDOW_H
#define OBJ_SHOPWINDOW_H

//Object Class
class ShopWindow : Object {

};

//Entity Class
class EntityShopWindow : Entity {

};

//Entity Functions
void ShopWindow_Update();
void ShopWindow_EarlyUpdate();
void ShopWindow_LateUpdate();
void ShopWindow_Draw();
void ShopWindow_Setup(void* subtype);
void ShopWindow_StageLoad();
void ShopWindow_GetAttributes();

#endif //!OBJ_SHOPWINDOW_H
