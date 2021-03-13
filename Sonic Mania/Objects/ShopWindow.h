#ifndef OBJ_SHOPWINDOW_H
#define OBJ_SHOPWINDOW_H

#include "../SonicMania.h"

// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectShopWindow;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityShopWindow;

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
