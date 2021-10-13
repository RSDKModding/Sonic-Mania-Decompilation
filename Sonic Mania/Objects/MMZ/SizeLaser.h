#ifndef OBJ_SIZELASER_H
#define OBJ_SIZELASER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    Hitbox playerOuterBox[4];
    Hitbox playerInnerBox[4];
    Vector2 playerPositions[4];
    uint16 spriteIndex;
    uint16 sonicIndex;
    uint16 tailsIndex;
    uint16 tailSpriteIndex;
    uint16 knuxIndex;
#if RETRO_USE_PLUS
    uint16 mightyIndex;
    uint16 rayIndex;
#endif
    uint16 sfxShrink2;
    uint16 sfxGrow2;
} ObjectSizeLaser;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    uint8 type;
    uint8 orientation;
    int32 timer;
    Vector2 storedPos;
    int32 extend;
    Animator animator1;
    Animator animator2;
} EntitySizeLaser;

// Object Struct
extern ObjectSizeLaser *SizeLaser;

// Standard Entity Events
void SizeLaser_Update(void);
void SizeLaser_LateUpdate(void);
void SizeLaser_StaticUpdate(void);
void SizeLaser_Draw(void);
void SizeLaser_Create(void* data);
void SizeLaser_StageLoad(void);
void SizeLaser_EditorDraw(void);
void SizeLaser_EditorLoad(void);
void SizeLaser_Serialize(void);

// Extra Entity Functions
bool32 SizeLaser_SizeChangeSFXCheck(void);

void SizeLaser_SetP2State(EntityPlayer *player, bool32 chibiFlag);
void SizeLaser_P2JumpInResize(void);
void SizeLaser_P2JumpInGrow(void);
void SizeLaser_P2JumpInShrink(void);
void SizeLaser_PlayerState_Grow(void);

void SizeLaser_Unknown3(void);
void SizeLaser_Unknown8(void);
void SizeLaser_Unknown9(void);
void SizeLaser_Unknown10(void);

#endif //!OBJ_SIZELASER_H
