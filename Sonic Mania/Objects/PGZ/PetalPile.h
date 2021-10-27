#ifndef OBJ_PETALPILE_H
#define OBJ_PETALPILE_H

#include "SonicMania.h"

// Object Class
typedef struct {
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
} ObjectPetalPile;

// Entity Class
typedef struct {
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
    int32 field_84;
    int32 field_88;
    int32 field_8C;
    int32 field_90;
    int8 field_94;
    int32 field_98;
    int32 field_9C;
    bool32 flag;
    Hitbox hitbox1;
    Animator animator;
} EntityPetalPile;

// Object Struct
extern ObjectPetalPile *PetalPile;

// Standard Entity Events
void PetalPile_Update(void);
void PetalPile_LateUpdate(void);
void PetalPile_StaticUpdate(void);
void PetalPile_Draw(void);
void PetalPile_Create(void *data);
void PetalPile_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void PetalPile_EditorDraw(void);
void PetalPile_EditorLoad(void);
#endif
void PetalPile_Serialize(void);

// Extra Entity Functions
int32 PetalPile_GetLeafPattern(int32 *patternPtr);

void PetalPile_State_Setup(void);
void PetalPile_State_HandleInteractions(void);
void PetalPile_State_Unknown3(void);
void PetalPile_State_Unknown4(void);

void PetalPile_SetupLeaf(void);
void PetalPile_State_Unknown5(void);
void PetalPile_State_Unknown6(void);
void PetalPile_State_Unknown7(void);

void PetalPile_StateDraw_Leaf(void);

#endif //! OBJ_PETALPILE_H
