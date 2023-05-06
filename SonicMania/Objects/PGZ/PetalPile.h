#ifndef OBJ_PETALPILE_H
#define OBJ_PETALPILE_H

#include "Game.h"

typedef enum {
    PETALPILE_FGHIGH,
    PETALPILE_FGLOW,
} PetalPileLayerIDs;

typedef enum {
    PETALPILE_PATTERN_0,
    PETALPILE_PATTERN_1,
    PETALPILE_PATTERN_2,
    PETALPILE_PATTERN_3,
    PETALPILE_PATTERN_4,
} PetalPilePatterns;

// Object Class
struct ObjectPetalPile {
    RSDK_OBJECT
    TABLE(int32 patternSize[5], { 7, 6, 6, 10, 3 });
    TABLE(int32 pattern1[14],
          { -0x10000, 0x10000, -0x8000, -0x10000, -0x5555, 0x10000, 0, -0x10000, 0x5555, 0x10000, 0x8000, -0x10000, 0x10000, 0x10000 });
    TABLE(int32 pattern2[12], { -0x10000, 0x10000, -0x5555, -0x5555, 0x10000, -0x10000, 0, 0xAAAA, 0xAAAA, 0, 0x10000, 0x10000 });
    TABLE(int32 pattern3[12], { 0x10000, 0x10000, 0x5555, -0x5555, -0x10000, -0x10000, 0, 0xAAAA, -0xAAAA, 0, -0x10000, 0x10000 });
    TABLE(int32 pattern4[20], { -0x10000, 0x10000,  -0x8000, -0x10000, -0x5555, 0x10000, 0, -0x10000, 0x5555, 0x10000,
                                0x8000,   -0x10000, 0x10000, 0x10000,  -0xAAAA, 0,       0, 0,        0xAAAA, 0 });
    TABLE(int32 pattern5[6], { 0, -0x10000, -0x10000, 0x10000, 0x10000, 0x10000 });
    uint16 aniFrames;
    uint16 sfxPetals;
};

// Entity Class
struct EntityPetalPile {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    uint16 timer;
    int8 leafPattern;
    int8 tileLayer;
    Vector2 pileSize;
    Vector2 maxSpeed;
    bool32 emitterMode;
    uint8 layerID;
    Vector2 velStore;
    Vector2 distance; // dunno if this is actually a vec2 but the .y is always set to 0 so maybe?
    int32 petalVel;
    int32 unused;
    int8 petalDir;
    int32 petalRadius;
    int32 petalOffset;
    bool32 noRemoveTiles;
    Hitbox hitbox;
    Animator animator;
};

// Object Struct
extern ObjectPetalPile *PetalPile;

// Standard Entity Events
void PetalPile_Update(void);
void PetalPile_LateUpdate(void);
void PetalPile_StaticUpdate(void);
void PetalPile_Draw(void);
void PetalPile_Create(void *data);
void PetalPile_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PetalPile_EditorDraw(void);
void PetalPile_EditorLoad(void);
#endif
void PetalPile_Serialize(void);

// Extra Entity Functions
int32 PetalPile_GetLeafPattern(Vector2 *patternPtr);

void PetalPile_State_Init(void);
void PetalPile_State_HandleInteractions(void);
void PetalPile_State_SetupEmitter(void);
void PetalPile_State_Emitter(void);

void PetalPile_StateLeaf_Setup(void);
void PetalPile_StateLeaf_Delay(void);
void PetalPile_StateLeaf_HandleVelocity(void);
void PetalPile_StateLeaf_Fall(void);

void PetalPile_Draw_Leaf(void);

#endif //! OBJ_PETALPILE_H
