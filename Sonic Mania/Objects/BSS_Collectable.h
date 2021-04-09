#ifndef OBJ_BSS_COLLECTABLE_H
#define OBJ_BSS_COLLECTABLE_H

#include "../SonicMania.h"

enum BSSCollectableTypes {
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
    BSS_UNKNOWN12     = 12,
    BSS_UNKNOWN13     = 13,
    BSS_UNKNOWN14     = 14,
    BSS_RING_SPARKLE  = 15,
    BSS_EMERALD_CHAOS = 16,
    BSS_EMERALD_SUPER = 17,
    BSS_MEDAL_SILVER  = 18,
    BSS_MEDAL_GOLD    = 19,
    BSS_UNKNOWN20     = 20,
    BSS_UNKNOWN21     = 21,
    BSS_UNKNOWN22     = 22,
    BSS_UNKNOWN23     = 23,

    BSS_SPHERE_GREEN_STOOD = 129,
    BSS_BLUE_STOOD = 130,
    BSS_SPHERE_PINK_STOOD = 134,
};

// Object Class
typedef struct {
    RSDK_OBJECT
    Animator sphereData[24];
    byte loadedTables;
    int scaleXTable[32]; //= { 2, 4, 4, 4, 6, 6, 6, 7, 7, 8, 8, 9, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 22, 24, 26, 28, 30, 32, 32, 32 };
    int scaleYTable[32]; //= { 2, 4, 4, 4, 6, 6, 6, 7, 7, 8, 8, 9, 9, 10, 11, 12, 13, 14, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20, 21, 22, 23, 24 };
    int scaleTable2[32]; //= { 4, 4, 5, 5, 6, 6, 7, 7, 8, 10, 12, 14, 16, 18, 20, 22, 24, 25, 26, 27, 28, 29, 30, 31, 32, 32, 32, 32, 32, 32, 32, 32
                         //};
    int screenYValues[32];
    int value7[32];
    ushort spriteIndex;
    ushort ringSpriteIndex;
} ObjectBSS_Collectable;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int type;
    int stateDraw;
    int frameID;
    int field_64;
    int field_68;
    int field_6C;
    int field_70;
} EntityBSS_Collectable;

// Object Struct
extern ObjectBSS_Collectable *BSS_Collectable;

// Standard Entity Events
void BSS_Collectable_Update(void);
void BSS_Collectable_LateUpdate(void);
void BSS_Collectable_StaticUpdate(void);
void BSS_Collectable_Draw(void);
void BSS_Collectable_Create(void *data);
void BSS_Collectable_StageLoad(void);
void BSS_Collectable_EditorDraw(void);
void BSS_Collectable_EditorLoad(void);
void BSS_Collectable_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_BSS_COLLECTABLE_H
