#ifndef OBJ_SIZELASER_H
#define OBJ_SIZELASER_H

#include "SonicMania.h"

typedef enum {
    SIZELASER_SHRINK,
    SIZELASER_GROW,
} SizeLaserTypes;

typedef enum {
    SIZELASER_ORIENTATION_DOWN,
    SIZELASER_ORIENTATION_RIGHT,
    SIZELASER_ORIENTATION_LEFT,
} SizeLaserOrientations;

// Object Class
struct ObjectSizeLaser {
    RSDK_OBJECT
    Hitbox hitbox;
    Hitbox playerOuterBox[4];
    Hitbox playerInnerBox[4];
    Vector2 playerPositions[4];
    uint16 aniFrames;
    uint16 sonicFrames;
    uint16 tailsFrames;
    uint16 tailFrames;
    uint16 knuxFrames;
#if MANIA_USE_PLUS
    uint16 mightyFrames;
    uint16 rayFrames;
#endif
    uint16 sfxShrink2;
    uint16 sfxGrow2;
};

// Entity Class
struct EntitySizeLaser {
    RSDK_ENTITY
    StateMachine(state);
    uint8 type;
    uint8 orientation;
    int32 timer;
    Vector2 impactPos;
    int32 extend;
    Animator mainAnimator;
    Animator impactAnimator;
};

// Object Struct
extern ObjectSizeLaser *SizeLaser;

// Standard Entity Events
void SizeLaser_Update(void);
void SizeLaser_LateUpdate(void);
void SizeLaser_StaticUpdate(void);
void SizeLaser_Draw(void);
void SizeLaser_Create(void* data);
void SizeLaser_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void SizeLaser_EditorDraw(void);
void SizeLaser_EditorLoad(void);
#endif
void SizeLaser_Serialize(void);

// Extra Entity Functions
bool32 SizeLaser_LaserSfxCheck(void);

void SizeLaser_SetP2State(EntityPlayer *player, bool32 chibiFlag);
void SizeLaser_P2JumpInResize(void);
void SizeLaser_P2JumpInGrow(void);
void SizeLaser_P2JumpInShrink(void);
void SizeLaser_PlayerState_Grow(void);

void SizeLaser_CheckPlayerCollisions(void);

void SizeLaser_State_Emitter(void);
void SizeLaser_State_Laser(void);
void SizeLaser_State_Impact(void);

#endif //!OBJ_SIZELASER_H
