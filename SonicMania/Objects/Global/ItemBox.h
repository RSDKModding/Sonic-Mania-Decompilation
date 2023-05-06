#ifndef OBJ_ITEMBOX_H
#define OBJ_ITEMBOX_H

#include "Game.h"

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
#if MANIA_USE_PLUS
    ITEMBOX_1UP_MIGHTY,
    ITEMBOX_1UP_RAY,
    ITEMBOX_STOCK,
#endif
    ITEMBOX_COUNT
} ItemBoxTypes;

// Object Class
struct ObjectItemBox {
    RSDK_OBJECT
    Hitbox hitboxItemBox;
    Hitbox hitboxHidden;
    int32 brokenFrame;
    uint16 aniFrames;
    uint16 sfxDestroy;
    uint16 sfxTeleport;
    uint16 sfxHyperRing;
#if MANIA_USE_PLUS
    uint16 sfxPowerDown;
    uint16 sfxRecovery;
#endif
};

// Entity Class
struct EntityItemBox {
    RSDK_ENTITY
    StateMachine(state);
    ItemBoxTypes type;
    Vector2 contentsPos;
    Vector2 moveOffset;
    int32 contentsSpeed;
    Entity *storedEntity;
    int32 timer;
    bool32 isFalling;
    bool32 isContents;
    bool32 hidden;
    PlaneFilterTypes planeFilter;
    bool32 lrzConvPhys;
    Animator boxAnimator;
    Animator contentsAnimator;
    Animator overlayAnimator;
    Animator debrisAnimator;
#if MANIA_USE_PLUS
    Entity *parent;
#endif
};

// Object Struct
extern ObjectItemBox *ItemBox;

// Standard Entity Events
void ItemBox_Update(void);
void ItemBox_LateUpdate(void);
void ItemBox_StaticUpdate(void);
void ItemBox_Draw(void);
void ItemBox_Create(void *data);
void ItemBox_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void ItemBox_EditorDraw(void);
void ItemBox_EditorLoad(void);
#endif
void ItemBox_Serialize(void);

// Extra Entity Functions
void ItemBox_DebugDraw(void);
void ItemBox_DebugSpawn(void);
void ItemBox_CheckHit(void);
void ItemBox_GivePowerup(void);
void ItemBox_Break(EntityItemBox *itemBox, EntityPlayer *player);
bool32 ItemBox_HandleFallingCollision(void);
bool32 ItemBox_HandlePlatformCollision(void *platform);
void ItemBox_HandleObjectCollisions(void);

// Entity States
void ItemBox_State_Broken(void);
void ItemBox_State_Break(void);
void ItemBox_State_IconFinish(void);
void ItemBox_State_Idle(void);
void ItemBox_State_Falling(void);
void ItemBox_State_Conveyor(void);

#endif //! OBJ_ITEMBOX_H
