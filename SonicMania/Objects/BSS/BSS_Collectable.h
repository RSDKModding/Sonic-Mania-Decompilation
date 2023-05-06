#ifndef OBJ_BSS_COLLECTABLE_H
#define OBJ_BSS_COLLECTABLE_H

#include "Game.h"

typedef enum {
    BSS_NONE          = 0,
    BSS_SPHERE_BLUE   = 1,
    BSS_SPHERE_RED    = 2,
    BSS_SPHERE_BUMPER = 3,
    BSS_SPHERE_YELLOW = 4,
    BSS_SPHERE_GREEN  = 5,
    BSS_SPHERE_PINK   = 6,
    BSS_RING          = 7,
    BSS_SPAWN_UP      = 8,
    BSS_SPAWN_RIGHT   = 9,
    BSS_SPAWN_DOWN    = 10,
    BSS_SPAWN_LEFT    = 11,
    BSS_UNUSED_1      = 12,
    BSS_UNUSED_2      = 13,
    BSS_UNUSED_3      = 14,
    BSS_RING_SPARKLE  = 15,
    BSS_EMERALD_CHAOS = 16,
    BSS_EMERALD_SUPER = 17,
    BSS_MEDAL_SILVER  = 18,
    BSS_MEDAL_GOLD    = 19,
    BSS_UNUSED_4      = 20,
    BSS_UNUSED_5      = 21,
    BSS_UNUSED_6      = 22,
    BSS_UNUSED_7      = 23,

    BSS_SPHERE_GREEN_STOOD = 0x80 | 1,
    BSS_BLUE_STOOD         = 0x80 | 2,
    BSS_SPHERE_PINK_STOOD  = 0x80 | 6,
} BSSCollectableTypes;

// Object Class
struct ObjectBSS_Collectable {
    RSDK_OBJECT
    Animator sphereAnimator[24];
    uint8 initializedTables;
    TABLE(int32 ringScaleTableX[32],
          { 2, 4, 4, 4, 6, 6, 6, 7, 7, 8, 8, 9, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 22, 24, 26, 28, 30, 32, 32, 32 });
    TABLE(int32 ringScaleTableY[32],
          { 2, 4, 4, 4, 6, 6, 6, 7, 7, 8, 8, 9, 9, 10, 11, 12, 13, 14, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20, 21, 22, 23, 24 });
    TABLE(int32 medalScaleTable[32],
          { 4, 4, 5, 5, 6, 6, 7, 7, 8, 10, 12, 14, 16, 18, 20, 22, 24, 25, 26, 27, 28, 29, 30, 31, 32, 32, 32, 32, 32, 32, 32, 32 });
    int32 screenYValues[32];
    int32 medalScreenYVals[32];
    uint16 aniFrames;
    uint16 ringFrames;
};

// Entity Class
struct EntityBSS_Collectable {
    RSDK_ENTITY
    int32 type;
    Animator animator;
};

// Object Struct
extern ObjectBSS_Collectable *BSS_Collectable;

// Standard Entity Events
void BSS_Collectable_Update(void);
void BSS_Collectable_LateUpdate(void);
void BSS_Collectable_StaticUpdate(void);
void BSS_Collectable_Draw(void);
void BSS_Collectable_Create(void *data);
void BSS_Collectable_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void BSS_Collectable_EditorDraw(void);
void BSS_Collectable_EditorLoad(void);
#endif
void BSS_Collectable_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_BSS_COLLECTABLE_H
