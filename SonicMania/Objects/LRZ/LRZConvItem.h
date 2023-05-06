#ifndef OBJ_LRZCONVITEM_H
#define OBJ_LRZCONVITEM_H

#include "Game.h"

typedef enum {
    LRZCONVITEM_ROCK,
    LRZCONVITEM_SPIKEBALL,
} LRZConvItemTypes;

// Object Class
struct ObjectLRZConvItem {
    RSDK_OBJECT
    Hitbox hitboxRock;
    Hitbox hitboxSpikeball;
    uint16 aniFrames;
    uint16 sfxSizzle;
};

// Entity Class
struct EntityLRZConvItem {
    RSDK_ENTITY
    StateMachine(state);
    uint8 type;
    Vector2 startPos;
    int32 unused;
    Animator animator;
};

// Object Struct
extern ObjectLRZConvItem *LRZConvItem;

// Standard Entity Events
void LRZConvItem_Update(void);
void LRZConvItem_LateUpdate(void);
void LRZConvItem_StaticUpdate(void);
void LRZConvItem_Draw(void);
void LRZConvItem_Create(void *data);
void LRZConvItem_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void LRZConvItem_EditorDraw(void);
void LRZConvItem_EditorLoad(void);
#endif
void LRZConvItem_Serialize(void);

// Extra Entity Functions
void LRZConvItem_SetupHitboxes(void);
Vector2 LRZConvItem_HandleLRZConvPhys(void *e);

void LRZConvItem_State_Rock(void);
void LRZConvItem_State_SpikeBall(void);

#endif //! OBJ_LRZCONVITEM_H
