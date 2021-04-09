#ifndef OBJ_AIZROCKPILE_H
#define OBJ_AIZROCKPILE_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    int rockPositions_small[8];
    int rockSpeedsT_small[8];
    int rockSpeedsL_small[8];
    int rockSpeedsR_small[8];
    int rockPositions_med[10];
    int rockSpeedsT_med[10];
    int rockSpeedsL_med[12];
    int rockSpeedsR_med[12];
    int rockPositions_large[16];
    int rockSpeedsT_large[16];
    int rockSpeedsL_large[16];
    int rockSpeedsR_large[16];
    ushort spriteIndex;
    ushort sfx_Break;
} ObjectAIZRockPile;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int size;
    bool32 smashTop;
    bool32 smashSides;
    bool32 onlyKnux;
    bool32 onlyMighty;
    int *rockPositions;
    int *rockSpeedsT;
    int *rockSpeedsL;
    int *rockSpeedsR;
    Animator data;
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
void AIZRockPile_SpawnRocks(int *speeds);

#endif

#endif //!OBJ_AIZROCKPILE_H
