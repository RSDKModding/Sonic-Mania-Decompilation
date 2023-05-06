#ifndef OBJ_AIZROCKPILE_H
#define OBJ_AIZROCKPILE_H

#include "Game.h"

#if MANIA_USE_PLUS

typedef enum {
    AIZROCKPILE_SMALL,
    AIZROCKPILE_MED,
    AIZROCKPILE_BIG,
} AIZRockPileTypes;

// Object Class
struct ObjectAIZRockPile {
    RSDK_OBJECT
    TABLE(int32 rockPositions_small[8], { -0x40000, -0x40000, 0xC0000, -0x40000, -0xC0000, 0x40000, 0xC0000, 0x40000 });
    TABLE(int32 rockSpeedsT_small[8], { -0x10000, -0x20000, 0x10000, -0x1E000, -0x1B000, -0x1C000, 0x1C000, -0x1C000 });
    TABLE(int32 rockSpeedsL_small[8], { -0x30000, -0x30000, -0x2C000, -0x28000, -0x2C000, -0x28000, -0x28000, -0x20000 });
    TABLE(int32 rockSpeedsR_small[8], { 0x2C000, -0x28000, 0x30000, -0x30000, 0x28000, -0x20000, 0x2C000, -0x28000 });
    TABLE(int32 rockPositions_med[10], { -0x40000, -0xC0000, 0xB0000, -0xC0000, -0x40000, -0x40000, -0xC0000, 0xC0000, 0xC0000, 0xC0000 });
    TABLE(int32 rockSpeedsT_med[10], { -0x30000, -0x30000, 0x2C000, -0x28000, -0x2C000, -0x28000, -0x28000, -0x20000, 0x28000, -0x18000 });
    TABLE(int32 rockSpeedsL_med[12],
          { -0x30000, -0x30000, -0x2C000, -0x28000, -0x2C000, -0x28000, -0x28000, -0x20000, -0x28000, -0x18000, -0x24000, -0x18000 });
    TABLE(int32 rockSpeedsR_med[12],
          { 0x2C000, -0x28000, 0x30000, -0x30000, 0x28000, -0x20000, 0x2C000, -0x28000, 0x24000, -0x18000, 0x28000, -0x18000 });
    TABLE(int32 rockPositions_large[16], { -0x80000, -0x180000, 0xB0000, -0x1C0000, -0x40000, -0xC0000, 0xC0000, -0x40000, -0xC0000, 0x40000, 0x40000,
                                           0xC0000, -0xC0000, 0x1C0000, 0xC0000, 0x1C0000 });
    TABLE(int32 rockSpeedsT_large[16], { -0x30000, -0x30000, 0x2C000, -0x28000, -0x2C000, -0x28000, 0x28000, -0x20000, -0x28000, -0x18000, 0x24000,
                                         -0x18000, -0x24000, -0x10000, 0x20000, -0x10000 });
    TABLE(int32 rockSpeedsL_large[16], { -0x30000, -0x30000, -0x2C000, -0x28000, -0x2C000, -0x28000, -0x28000, -0x20000, -0x28000, -0x18000, -0x24000,
                                         -0x18000, -0x24000, -0x10000, -0x20000, -0x10000 });
    TABLE(int32 rockSpeedsR_large[16], { 0x2C000, -0x28000, 0x30000, -0x30000, 0x28000, -0x20000, 0x2C000, -0x28000, 0x24000, -0x18000, 0x28000,
                                         -0x18000, 0x20000, -0x10000, 0x24000, -0x10000 });
    uint16 aniFrames;
    uint16 sfxBreak;
};

// Entity Class
struct EntityAIZRockPile {
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
};

// Object Struct
extern ObjectAIZRockPile *AIZRockPile;

// Standard Entity Events
void AIZRockPile_Update(void);
void AIZRockPile_LateUpdate(void);
void AIZRockPile_StaticUpdate(void);
void AIZRockPile_Draw(void);
void AIZRockPile_Create(void *data);
void AIZRockPile_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void AIZRockPile_EditorDraw(void);
void AIZRockPile_EditorLoad(void);
#endif
void AIZRockPile_Serialize(void);

// Extra Entity Functions
void AIZRockPile_SpawnRocks(int32 *speeds);

#endif

#endif //! OBJ_AIZROCKPILE_H
