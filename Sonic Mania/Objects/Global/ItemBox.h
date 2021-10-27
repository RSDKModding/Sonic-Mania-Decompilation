#ifndef OBJ_ITEMBOX_H
#define OBJ_ITEMBOX_H

#include "SonicMania.h"

typedef enum {
    ITEMBOX_RING,
    ITEMBOX_BLUESHIELD,
    ITEMBOX_BUBBLESHIELD,
    ITEMBOX_FIRESHIELD,
    ITEMBOX_LIGHTNINGSHIELD,
    ITEMBOX_INVINCIBLE,
    ITEMBOX_SNEAKERS,
    ITEMBOX_1UP_SONIC,
    ITEMBOX_1UP_TAILS,
    ITEMBOX_1UP_KNUX,
    ITEMBOX_EGGMAN,
    ITEMBOX_HYPERRING,
    ITEMBOX_SWAP,
    ITEMBOX_RANDOM,
    ITEMBOX_SUPER,
#if RETRO_USE_PLUS
    ITEMBOX_1UP_MIGHTY,
    ITEMBOX_1UP_RAY,
    ITEMBOX_STOCK,
#endif
    ITEMBOX_COUNT
}ItemBoxTypes;

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    Hitbox hiddenHitbox;
    int32 brokenFrame;
    uint16 spriteIndex;
    uint16 sfx_Destroy;
    uint16 sfx_Teleport;
    uint16 sfx_HyperRing;
    uint16 sfx_PowerDown;
    uint16 sfx_Revovery;
} ObjectItemBox;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    ItemBoxTypes type;
    Vector2 contentsPos;
    Vector2 unknownPos;
    int32 contentsSpeed;
    Entity *storedEntity;
    int32 timer;
    bool32 isFalling;
    bool32 isContents;
    bool32 hidden;
    PlaneFilterTypes planeFilter;
    bool32 lrzConvPhys;
    Animator animatorBox;
    Animator animatorContents;
    Animator animatorOverlay;
    Animator animatorDebris;
    Entity *parent;
} EntityItemBox;

// Object Struct
extern ObjectItemBox *ItemBox;

// Standard Entity Events
void ItemBox_Update(void);
void ItemBox_LateUpdate(void);
void ItemBox_StaticUpdate(void);
void ItemBox_Draw(void);
void ItemBox_Create(void* data);
void ItemBox_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void ItemBox_EditorDraw(void);
void ItemBox_EditorLoad(void);
#endif
void ItemBox_Serialize(void);

// Extra Entity Functions
void ItemBox_DebugDraw(void);
void ItemBox_DebugSpawn(void);
void ItemBox_CheckHit(void);
void ItemBox_GivePowerup(void);
void ItemBox_Break(EntityItemBox *itemBox, void *p);
bool32 ItemBox_HandleFallingCollision(void);
bool32 ItemBox_HandlePlatformCollision(void *p);
void ItemBox_HandleObjectCollisions(void);

//Entity States
void ItemBox_State_Broken(void);
void ItemBox_State_Contents(void);
void ItemBox_State_Unknown(void);
void ItemBox_State_Normal(void);
void ItemBox_State_Falling(void);
void ItemBox_State_Conveyor(void);

#endif //!OBJ_ITEMBOX_H
