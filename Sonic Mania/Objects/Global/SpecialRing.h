#ifndef OBJ_SPECIALRING_H
#define OBJ_SPECIALRING_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 spriteIndex;
    Hitbox hitbox;
    uint16 sfx_SpecialRing;
    uint16 sfx_SpecialWarp;
    uint16 modelIndex;
    uint16 sceneIndex;
} ObjectSpecialRing;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 id;
    int32 planeFilter;
    int32 warpTimer;
    int32 dword68;
    Animator warpData;
    int32 angleZ;
    int32 angleY;
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
