#ifndef OBJ_SPIKECRUSHER_H
#define OBJ_SPIKECRUSHER_H

#include "Game.h"

// Object Class
struct ObjectSpikeCrusher {
    RSDK_OBJECT
    uint16 sfxHuff;
    uint16 sfxImpact;
};

// Entity Class
struct EntitySpikeCrusher {
    MANIA_PLATFORM_BASE
    int32 startDir;
    Animator spikeAnimator;
};

// Object Struct
extern ObjectSpikeCrusher *SpikeCrusher;

// Standard Entity Events
void SpikeCrusher_Update(void);
void SpikeCrusher_LateUpdate(void);
void SpikeCrusher_StaticUpdate(void);
void SpikeCrusher_Draw(void);
void SpikeCrusher_Create(void *data);
void SpikeCrusher_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void SpikeCrusher_EditorDraw(void);
void SpikeCrusher_EditorLoad(void);
#endif
void SpikeCrusher_Serialize(void);

// Extra Entity Functions
void SpikeCrusher_CheckOffScreen(void);

void SpikeCrusher_State_Init(void);
void SpikeCrusher_State_MovingDown(void);
void SpikeCrusher_State_Crushing(void);
void SpikeCrusher_State_CrushBounce(void);
void SpikeCrusher_State_ActivateFlames(void);
void SpikeCrusher_State_MovingUp(void);

#endif //! OBJ_SPIKECRUSHER_H
