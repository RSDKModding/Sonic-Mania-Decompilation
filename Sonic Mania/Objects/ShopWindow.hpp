#ifndef OBJ_SHOPWINDOW_H
#define OBJ_SHOPWINDOW_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectShopWindow : Object {

};

// Entity Class
struct EntityShopWindow : Entity {

};

// Object Struct
extern ObjectShopWindow *ShopWindow;

// Standard Entity Events
void ShopWindow_Update();
void ShopWindow_LateUpdate();
void ShopWindow_StaticUpdate();
void ShopWindow_Draw();
void ShopWindow_Create(void* data);
void ShopWindow_StageLoad();
void ShopWindow_EditorDraw();
void ShopWindow_EditorLoad();
void ShopWindow_Serialize();

// Extra Entity Functions


#endif //!OBJ_SHOPWINDOW_H
