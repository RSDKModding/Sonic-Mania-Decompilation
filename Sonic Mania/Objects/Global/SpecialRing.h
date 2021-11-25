#ifndef OBJ_SPECIALRING_H
#define OBJ_SPECIALRING_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitbox;
    uint16 sfxSpecialRing;
    uint16 sfxSpecialWarp;
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
    int32 sparkleRadius;
    Animator warpAnimator;
    int32 angleZ;
    int32 angleY;
    bool32 enabled;
    Matrix matTempRot;
    Matrix matTransform;
    Matrix matWorld;
    Matrix matNormal;
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
#if RETRO_INCLUDE_EDITOR
void SpecialRing_EditorDraw(void);
void SpecialRing_EditorLoad(void);
#endif
void SpecialRing_Serialize(void);

// Extra Entity Functions
void SpecialRing_DebugDraw(void);
void SpecialRing_DebugSpawn(void);

void SpecialRing_State_StartWarp(void);
void SpecialRing_State_Warp(void);
void SpecialRing_State_Normal(void);

#endif //!OBJ_SPECIALRING_H
