#ifndef OBJ_WHIRLPOOL_H
#define OBJ_WHIRLPOOL_H

#include "Game.h"

// Object Class
struct ObjectWhirlpool {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxWhirlpool;
    bool32 playingWhirlpoolSfx;
    int32 sfxChannel;
    int32 timer;
};

// Entity Class
struct EntityWhirlpool {
    RSDK_ENTITY
    int32 angVel;
    int32 yVel;
    Vector2 size;
    uint8 activePlayers;
    Vector2 startPos;
    int32 unused;
    int32 bubbleAngles[128];
    int32 playerAngle[PLAYER_COUNT];
    int32 playerAmplitude[PLAYER_COUNT];
    Hitbox hitbox;
    Animator animator;
};

// Object Struct
extern ObjectWhirlpool *Whirlpool;

// Standard Entity Events
void Whirlpool_Update(void);
void Whirlpool_LateUpdate(void);
void Whirlpool_StaticUpdate(void);
void Whirlpool_Draw(void);
void Whirlpool_Create(void *data);
void Whirlpool_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Whirlpool_EditorDraw(void);
void Whirlpool_EditorLoad(void);
#endif
void Whirlpool_Serialize(void);

// Extra Entity Functions
void WhirlPool_DrawSprites(void);
void Whirlpool_SetupBubbles(void);

#endif //! OBJ_WHIRLPOOL_H
