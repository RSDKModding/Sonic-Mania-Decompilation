#ifndef OBJ_SIZELASER_H
#define OBJ_SIZELASER_H

#include "Game.h"

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
    Hitbox playerOuterBox[PLAYER_COUNT];
    Hitbox playerInnerBox[PLAYER_COUNT];
    Vector2 playerPositions[PLAYER_COUNT];
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
void SizeLaser_Create(void *data);
void SizeLaser_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void SizeLaser_EditorDraw(void);
void SizeLaser_EditorLoad(void);
#endif
void SizeLaser_Serialize(void);

// Extra Entity Functions
bool32 SizeLaser_SfxCheck_SizeLaser(void);

void SizeLaser_SetPlayerSize(EntityPlayer *player, bool32 chibiFlag);
void SizeLaser_PlayerState_Resize(void);
void SizeLaser_PlayerState_GrowNormal(void);
void SizeLaser_PlayerState_ShrinkChibi(void);
void SizeLaser_PlayerState_GrowGiant(void);

void SizeLaser_CheckPlayerCollisions(void);

void SizeLaser_State_Emitter(void);
void SizeLaser_State_Laser(void);
void SizeLaser_State_Impact(void);

#endif //! OBJ_SIZELASER_H
