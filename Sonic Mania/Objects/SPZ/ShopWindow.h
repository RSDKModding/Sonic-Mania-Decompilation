#ifndef OBJ_SHOPWINDOW_H
#define OBJ_SHOPWINDOW_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 spriteIndex;
    uint16 sfxShatter;
} ObjectShopWindow;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    Vector2 size;
    bool32 shatter;
    bool32 silhouette;
    Hitbox hitbox;
    Hitbox windowHitbox;
    Hitbox hitbox2;
    Vector2 field_88;
    Vector2 field_90;
    Animator animator;
} EntityShopWindow;

// Object Struct
extern ObjectShopWindow *ShopWindow;

// Standard Entity Events
void ShopWindow_Update(void);
void ShopWindow_LateUpdate(void);
void ShopWindow_StaticUpdate(void);
void ShopWindow_Draw(void);
void ShopWindow_Create(void* data);
void ShopWindow_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void ShopWindow_EditorDraw(void);
void ShopWindow_EditorLoad(void);
#endif
void ShopWindow_Serialize(void);

// Extra Entity Functions
void ShopWindow_State_Shard(void);
void ShopWindow_State_Shattered(void);
void ShopWindow_State_Silhouette(void);
void ShopWindow_Draw_Normal(void);
void ShopWindow_State_Draw_Shard(void);
void ShopWindow_State_Draw_Shattered(void);

#endif //!OBJ_SHOPWINDOW_H
