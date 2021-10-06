#ifndef OBJ_ICE_H
#define OBJ_ICE_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
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
}IceAni;
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

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort aniFrames;
    Hitbox hitbox1;
    Hitbox hitbox2;
    int playerTimers[PLAYER_MAX];
    ushort sfxFreeze;
    ushort sfxLedgeBreak;
    ushort sfxWindowShatter;
    ushort sfxStruggle;
} ObjectIce;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    byte size;
    byte type;
    byte subType;
    byte subFlip;
    bool32 bottomSmash;
    bool32 knuxSmash;
    byte timer;
    byte animationID;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    Entity *playerPtr;
    Vector2 playerPos;
    Vector2 dwordDC;
    int dwordE4;
    Hitbox hitbox1;
    Hitbox hitbox2;
    Hitbox hitbox3;
} EntityIce;

// Object Struct
extern ObjectIce *Ice;

// Standard Entity Events
void Ice_Update(void);
void Ice_LateUpdate(void);
void Ice_StaticUpdate(void);
void Ice_Draw(void);
void Ice_Create(void *data);
void Ice_StageLoad(void);
void Ice_EditorDraw(void);
void Ice_EditorLoad(void);
void Ice_Serialize(void);

// Extra Entity Functions
void Ice_ZoneCB(void);

void Ice_FreezePlayer(void *p);
bool32 Ice_Unknown3(void);
bool32 Ice_Unknown4(void);
void Ice_State_FrozenPlayer(void);
void Ice_ShatterGenerator(int xr, int xy, int count, int velX, int velY, int a6);
void Ice_Unknown7(int velX, Entity *p, int velY);
void Ice_Unknown8(Entity *p);
Entity *Ice_Shatter(EntityIce *ice, int velX, int velY);
void Ice_TimeOverCB(void);

void Ice_Unknown11(void);
void Ice_State_Pillar(void);
void Ice_Unknown13(void);
void Ice_Unknown14(void);
void Ice_Unknown15(void);
void Ice_State_PlayerBlock(void);
void Ice_State_Shard(void);

void Ice_StateDraw_Unknown1(void);
void Ice_StateDraw_PlayerBlock(void);
void Ice_StateDraw_Pillar(void);
void Ice_StateDraw_Shard(void);

#endif //! OBJ_ICE_H
