#ifndef OBJ_SPIKEFLAIL_H
#define OBJ_SPIKEFLAIL_H

#include "Game.h"

// Object Class
struct ObjectSpikeFlail {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxFlail;
};

// Entity Class
struct EntitySpikeFlail {
    RSDK_ENTITY
    uint8 chainLength;
    uint16 phase512;
    Animator poleAnimator;
    Animator spikeBallAnimator;
    Animator ringAnimator;
    Vector2 origin;
    bool32 playingFlailSfx;
    Hitbox hitbox;
};

// Object Struct
extern ObjectSpikeFlail *SpikeFlail;

// Standard Entity Events
void SpikeFlail_Update(void);
void SpikeFlail_LateUpdate(void);
void SpikeFlail_StaticUpdate(void);
void SpikeFlail_Draw(void);
void SpikeFlail_Create(void *data);
void SpikeFlail_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void SpikeFlail_EditorDraw(void);
void SpikeFlail_EditorLoad(void);
#endif
void SpikeFlail_Serialize(void);

// Extra Entity Functions
void SpikeFlail_SetupHitbox(void);
void SpikeFlail_CheckPlayerCollisions(void);
Vector2 SpikeFlail_GetScale(int32 ballPos, int32 angle);

#endif //! OBJ_SPIKEFLAIL_H
