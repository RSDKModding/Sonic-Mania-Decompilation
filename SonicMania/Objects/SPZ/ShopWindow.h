#ifndef OBJ_SHOPWINDOW_H
#define OBJ_SHOPWINDOW_H

#include "Game.h"

// Object Class
struct ObjectShopWindow {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxShatter;
};

// Entity Class
struct EntityShopWindow {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    Vector2 size;
    bool32 shatter;
    bool32 silhouette;
    Hitbox hitboxWindowX;
    Hitbox hitboxWindowY;
    Hitbox hitboxItem;
    Vector2 scaleSpeed;
    Vector2 shardScale;
    Animator animator;
};

// Object Struct
extern ObjectShopWindow *ShopWindow;

// Standard Entity Events
void ShopWindow_Update(void);
void ShopWindow_LateUpdate(void);
void ShopWindow_StaticUpdate(void);
void ShopWindow_Draw(void);
void ShopWindow_Create(void *data);
void ShopWindow_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void ShopWindow_EditorDraw(void);
void ShopWindow_EditorLoad(void);
#endif
void ShopWindow_Serialize(void);

// Extra Entity Functions
void ShopWindow_State_Shard(void);
void ShopWindow_State_Shattered(void);
void ShopWindow_State_Silhouette(void);
void ShopWindow_Draw_Normal(void);
void ShopWindow_Draw_Shard(void);
void ShopWindow_Draw_Shattered(void);

#endif //! OBJ_SHOPWINDOW_H
