#ifndef OBJ_SHOPWINDOW_H
#define OBJ_SHOPWINDOW_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort spriteIndex;
    ushort sfxShatter;
} ObjectShopWindow;

// Entity Class
typedef struct {
    RSDK_ENTITY
    void(*state)();
    void(*stateDraw)();
    Vector2 size;
    bool32 shatter;
    bool32 silhouette;
    Hitbox hitbox;
    Hitbox windowHitbox;
    Hitbox hitbox2;
    Vector2 field_88;
    Vector2 field_90;
    AnimationData data;
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
void ShopWindow_State_Shard();
void ShopWindow_State_Shattered();
void ShopWindow_State_Silhouette();
void ShopWindow_Draw_Normal();
void ShopWindow_State_Draw_Shard();
void ShopWindow_State_Draw_Shattered();

#endif //!OBJ_SHOPWINDOW_H
