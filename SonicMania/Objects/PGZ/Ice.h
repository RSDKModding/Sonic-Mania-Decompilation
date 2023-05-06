#ifndef OBJ_ICE_H
#define OBJ_ICE_H

#include "Game.h"

#if MANIA_USE_PLUS
typedef enum {
    ICEANI_ICEBLOCK,
    ICEANI_PLAYERBLOCK,
    ICEANI_PLAYERGLINT,
    ICEANI_PILLARBLOCK,
    ICEANI_PILLARGLINT,
    ICEANI_SHARD,
    ICEANI_LARGEGLINT,
    ICEANI_SMALLGLINT,
    ICEANI_PIECE,
    ICEANI_RINGS,
    ICEANI_SONICIDLE,
    ICEANI_SONICLEFT,
    ICEANI_SONICRIGHT,
    ICEANI_SONICSHAKE,
    ICEANI_SUPERSONICIDLE,
    ICEANI_SUPERSONICLEFT,
    ICEANI_SUPERSONICRIGHT,
    ICEANI_SUPERSONICSHAKE,
    ICEANI_TAILSIDLE,
    ICEANI_TAILSLEFT,
    ICEANI_TAILSRIGHT,
    ICEANI_TAILSSHAKE,
    ICEANI_KNUXIDLE,
    ICEANI_KNUXLEFT,
    ICEANI_KNUXRIGHT,
    ICEANI_KNUXSHAKE,
    ICEANI_MIGHTYIDLE,
    ICEANI_MIGHTYLEFT,
    ICEANI_MIGHTYRIGHT,
    ICEANI_MIGHTYSHAKE,
    ICEANI_RAYIDLE,
    ICEANI_RAYLEFT,
    ICEANI_RAYRIGHT,
    ICEANI_RAYSHAKE
} IceAni;
#else
typedef enum {
    ICEANI_ICEBLOCK,
    ICEANI_PLAYERBLOCK,
    ICEANI_RINGS,
    ICEANI_SONIC,
    ICEANI_TAILS,
    ICEANI_KNUX,
    ICEANI_PILLARBLOCK,
    ICEANI_PLAYERGLINT,
    ICEANI_PILLARGLINT,
    ICEANI_SHARD,
    ICEANI_LARGEGLINT,
    ICEANI_SMALLGLINT,
    ICEANI_PIECE
} IceAni;
#endif

typedef enum {
    ICE_SIZE_LARGE,
    ICE_SIZE_SMALL,
} IceSizes;

typedef enum {
    ICE_BLOCK,
    ICE_1RING,
    ICE_3RINGS,
    ICE_5RINGS,
    ICE_SPIKES,
    ICE_ITEMBOX_RINGS,
    ICE_ITEMBOX_BLUESHIELD,
    ICE_ITEMBOX_BUBBLESHIELD,
    ICE_ITEMBOX_FIRESHIELD,
    ICE_ITEMBOX_LIGHTNINGSHIELD,
    ICE_ITEMBOX_INVINCIBILITY,
    ICE_ITEMBOX_SNEAKERS,
    ICE_ITEMBOX_1UP,
    ICE_ITEMBOX_EGGMAN,
    ICE_ITEMBOX_HYPERRING,
    ICE_ITEMBOX_SWAP,
    ICE_ITEMBOX_RANDOM,
    ICE_ITEMBOX_SUPER,
    ICE_SPRING,
} IceTypes;

typedef enum {
    ICE_CHILD_NONE,
    ICE_CHILD_PLAYER,
    ICE_CHILD_PILLAR,
    ICE_CHILD_SHARD,
} IceChildTypes;

// Object Class
struct ObjectIce {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitboxPlayerBlockInner;
    Hitbox hitboxPlayerBlockOuter;
    int32 playerTimers[PLAYER_COUNT];
    uint16 sfxFreeze;
    uint16 sfxLedgeBreak;
    uint16 sfxWindowShatter;
    uint16 sfxStruggle;
};

// Entity Class
struct EntityIce {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    uint8 size;
    uint8 type;
    uint8 subType;
    uint8 subFlip;
    bool32 bottomSmash;
    bool32 knuxSmash;
    uint8 timer;
    uint8 animationID;
    Animator blockAnimator;
    Animator contentsAnimator;
    Animator altContentsAnimator;
    Animator glintAnimator;
    EntityPlayer *playerPtr;
    Vector2 playerMoveOffset;
    Vector2 contentsOffset;
    int32 glintTimer;
    Hitbox hitboxBlock;
    Hitbox hitboxFallCheck;
    Hitbox hitboxPlayerBlockCheck;
};

// Object Struct
extern ObjectIce *Ice;

// Standard Entity Events
void Ice_Update(void);
void Ice_LateUpdate(void);
void Ice_StaticUpdate(void);
void Ice_Draw(void);
void Ice_Create(void *data);
void Ice_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Ice_EditorDraw(void);
void Ice_EditorLoad(void);
#endif
void Ice_Serialize(void);

// Extra Entity Functions
void Ice_VSSwap_CheckFrozen(void);

void Ice_FreezePlayer(EntityPlayer *player);
bool32 Ice_CheckPlayerBlockSmashH(void);
bool32 Ice_CheckPlayerBlockSmashV(void);
void Ice_PlayerState_Frozen(void);
void Ice_ShatterGenerator(int32 count, int32 sizeX, int32 sizeY, int32 velX, int32 velY, int32 canBreak);
void Ice_FullShatter(EntityPlayer *player, int32 velX, int32 velY);
void Ice_BreakPlayerBlock(EntityPlayer *player);
EntityItemBox *Ice_Shatter(EntityIce *ice, int32 velX, int32 velY);
void Ice_TimeOver_CheckFrozen(void);

void Ice_UpdateBlockGravity(void);
void Ice_State_IceBlock(void);
void Ice_State_StartBlockFall(void);
void Ice_State_BlockFallDelay(void);
void Ice_State_IceBlockFall(void);
void Ice_State_PlayerBlock(void);
void Ice_State_Shard(void);

void Ice_Draw_IceBlock(void);
void Ice_Draw_PlayerBlock(void);
void Ice_Draw_Pillar(void);
void Ice_Draw_Shard(void);

#endif //! OBJ_ICE_H
