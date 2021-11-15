#ifndef OBJ_LRZCONVITEM_H
#define OBJ_LRZCONVITEM_H

#include "SonicMania.h"

typedef enum {
    LRZCONVITEM_ROCK,
    LRZCONVITEM_SPIKEBALL,
}LRZConvItemTypes;

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox1;
    Hitbox hitbox2;
    uint16 aniFrames;
    uint16 sfxSizzle;
} ObjectLRZConvItem;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    uint8 type;
    Vector2 startPos;
    int32 field_68;
    Animator animator;
} EntityLRZConvItem;

// Object Struct
extern ObjectLRZConvItem *LRZConvItem;

// Standard Entity Events
void LRZConvItem_Update(void);
void LRZConvItem_LateUpdate(void);
void LRZConvItem_StaticUpdate(void);
void LRZConvItem_Draw(void);
void LRZConvItem_Create(void* data);
void LRZConvItem_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void LRZConvItem_EditorDraw(void);
void LRZConvItem_EditorLoad(void);
#endif
void LRZConvItem_Serialize(void);

// Extra Entity Functions
void LRZConvItem_SetupHitboxes(void);
Vector2 LRZConvItem_Unknown2(void *e);

void LRZConvItem_State_Unknown1(void);
void LRZConvItem_State_Unknown2(void);

#endif //!OBJ_LRZCONVITEM_H
