#ifndef OBJ_WHIRLPOOL_H
#define OBJ_WHIRLPOOL_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxWhirlpool;
    bool32 playingSFX;
    int sfxChannel;
    int timer;
} ObjectWhirlpool;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int angVel;
    int yVel;
    Vector2 size;
    uint8 activePlayers;
    Vector2 posUnknown2;
    int field_74;
    int field_78[128];
    int field_278[4];
    int field_288[4];
    Hitbox hitbox;
    Animator animator;
} EntityWhirlpool;

// Object Struct
extern ObjectWhirlpool *Whirlpool;

// Standard Entity Events
void Whirlpool_Update(void);
void Whirlpool_LateUpdate(void);
void Whirlpool_StaticUpdate(void);
void Whirlpool_Draw(void);
void Whirlpool_Create(void* data);
void Whirlpool_StageLoad(void);
void Whirlpool_EditorDraw(void);
void Whirlpool_EditorLoad(void);
void Whirlpool_Serialize(void);

// Extra Entity Functions
void WhirlPool_DrawSprites(void);
void Whirlpool_SetupBubbles(void);

#endif //!OBJ_WHIRLPOOL_H
