#ifndef OBJ_SPIKECRUSHER_H
#define OBJ_SPIKECRUSHER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 sfxHuff;
    uint16 sfxImpact;
} ObjectSpikeCrusher;

// Entity Class
typedef struct {
    MANIA_PLATFORM_BASE
    int32 startDir;
    Animator animator2;
} EntitySpikeCrusher;

// Object Struct
extern ObjectSpikeCrusher *SpikeCrusher;

// Standard Entity Events
void SpikeCrusher_Update(void);
void SpikeCrusher_LateUpdate(void);
void SpikeCrusher_StaticUpdate(void);
void SpikeCrusher_Draw(void);
void SpikeCrusher_Create(void* data);
void SpikeCrusher_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void SpikeCrusher_EditorDraw(void);
void SpikeCrusher_EditorLoad(void);
#endif
void SpikeCrusher_Serialize(void);

// Extra Entity Functions
void SpikeCrusher_CheckOnScreen(void);

void SpikeCrusher_State_Unknown1(void);
void SpikeCrusher_State_Unknown2(void);
void SpikeCrusher_State_Unknown3(void);
void SpikeCrusher_State_Unknown4(void);
void SpikeCrusher_State_Unknown5(void);
void SpikeCrusher_State_Unknown6(void);

#endif //!OBJ_SPIKECRUSHER_H
