#ifndef OBJ_SPECIALRING_H
#define OBJ_SPECIALRING_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort spriteIndex;
    Hitbox hitbox;
    ushort sfx_SpecialRing;
    ushort sfx_SpecialWarp;
    ushort modelIndex;
    ushort sceneIndex;
} ObjectSpecialRing;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int id;
    int planeFilter;
    int warpTimer;
    int dword68;
    Animator warpData;
    int angleZ;
    int angleY;
    bool32 enabled;
    Matrix matrix4;
    Matrix matrix;
    Matrix matrix2;
    Matrix matrix3;
} EntitySpecialRing;

// Object Struct
extern ObjectSpecialRing *SpecialRing;

// Standard Entity Events
void SpecialRing_Update(void);
void SpecialRing_LateUpdate(void);
void SpecialRing_StaticUpdate(void);
void SpecialRing_Draw(void);
void SpecialRing_Create(void* data);
void SpecialRing_StageLoad(void);
void SpecialRing_EditorDraw(void);
void SpecialRing_EditorLoad(void);
void SpecialRing_Serialize(void);

// Extra Entity Functions
void SpecialRing_DebugDraw(void);
void SpecialRing_DebugSpawn(void);

void SpecialRing_StartWarp(void);
void SpecialRing_State_Warp(void);
void SpecialRing_State_Normal(void);

#endif //!OBJ_SPECIALRING_H
