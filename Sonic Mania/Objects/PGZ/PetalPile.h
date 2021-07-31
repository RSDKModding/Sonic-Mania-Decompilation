#ifndef OBJ_PETALPILE_H
#define OBJ_PETALPILE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int patternSize[5]; //= { 7, 6, 6, 10, 3 };
    int pattern1[14];   //= { -65536, 65536, -32768, -65536, -21845, 65536, 0, -65536, 21845, 65536, 32768, -65536, 65536, 65536 };
    int pattern2[12];   //= { -65536, 65536, -21845, -21845, 65536, -65536, 0, 43690, 43690, 0, 65536, 65536 };
    int pattern3[12];   //= { 65536, 65536, 21845, -21845, -65536, -65536, 0, 43690, -43690, 0, -65536, 65536 };
    int pattern4[20];   //= { -65536, 65536, -32768, -65536, -21845, 65536, 0, -65536, 21845, 65536, 32768, -65536, 65536, 65536, -43690, 0, 0, 0,
                        //43690, 0 };
    int pattern5[6];    //= { 0, -65536, -65536, 65536, 65536, 65536 };
    ushort aniFrames;
    ushort sfxPetals;
} ObjectPetalPile;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    ushort timer;
    sbyte leafPattern;
    sbyte tileLayer;
    Vector2 pileSize;
    Vector2 maxSpeed;
    bool32 emitterMode;
    byte layerID;
    Vector2 velStore;
    int field_84;
    int field_88;
    int field_8C;
    int field_90;
    sbyte field_94;
    int field_98;
    int field_9C;
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
void PetalPile_EditorDraw(void);
void PetalPile_EditorLoad(void);
void PetalPile_Serialize(void);

// Extra Entity Functions
int PetalPile_GetLeafPattern(int *patternPtr);

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
