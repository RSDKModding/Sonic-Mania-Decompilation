#ifndef OBJ_AIZROCKPILE_H
#define OBJ_AIZROCKPILE_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    int32 rockPositions_small[8];
    int32 rockSpeedsT_small[8];
    int32 rockSpeedsL_small[8];
    int32 rockSpeedsR_small[8];
    int32 rockPositions_med[10];
    int32 rockSpeedsT_med[10];
    int32 rockSpeedsL_med[12];
    int32 rockSpeedsR_med[12];
    int32 rockPositions_large[16];
    int32 rockSpeedsT_large[16];
    int32 rockSpeedsL_large[16];
    int32 rockSpeedsR_large[16];
    uint16 spriteIndex;
    uint16 sfx_Break;
} ObjectAIZRockPile;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int32 size;
    bool32 smashTop;
    bool32 smashSides;
    bool32 onlyKnux;
    bool32 onlyMighty;
    int32 *rockPositions;
    int32 *rockSpeedsT;
    int32 *rockSpeedsL;
    int32 *rockSpeedsR;
    Animator animator;
} EntityAIZRockPile;

// Object Struct
extern ObjectAIZRockPile *AIZRockPile;

// Standard Entity Events
void AIZRockPile_Update(void);
void AIZRockPile_LateUpdate(void);
void AIZRockPile_StaticUpdate(void);
void AIZRockPile_Draw(void);
void AIZRockPile_Create(void* data);
void AIZRockPile_StageLoad(void);
void AIZRockPile_EditorDraw(void);
void AIZRockPile_EditorLoad(void);
void AIZRockPile_Serialize(void);

// Extra Entity Functions
void AIZRockPile_SpawnRocks(int32 *speeds);

#endif

#endif //!OBJ_AIZROCKPILE_H
