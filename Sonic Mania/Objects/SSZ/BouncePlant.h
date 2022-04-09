#ifndef OBJ_BOUNCEPLANT_H
#define OBJ_BOUNCEPLANT_H

#include "SonicMania.h"

#define BouncePlant_NodeCount (8)

// Object Class
struct ObjectBouncePlant {
    RSDK_OBJECT
    Hitbox hitbox;
    uint16 aniFrames;
    uint16 sfxBouncePlant;
};

// Entity Class
struct EntityBouncePlant {
    RSDK_ENTITY
    StateMachine(state);
    int32 recoilDuration;
    Vector2 drawPos[BouncePlant_NodeCount];
    Vector2 nodeStartPos[BouncePlant_NodeCount];
    int32 recoilVelocity[BouncePlant_NodeCount];
    Vector2 stoodPos;
    bool32 stood;
    bool32 instantRecoil;
    int32 depression;
    int32 centerX;
    Animator nodeAnimator;
    Animator decorAnimators[BouncePlant_NodeCount];
};

// Object Struct
extern ObjectBouncePlant *BouncePlant;

// Standard Entity Events
void BouncePlant_Update(void);
void BouncePlant_LateUpdate(void);
void BouncePlant_StaticUpdate(void);
void BouncePlant_Draw(void);
void BouncePlant_Create(void* data);
void BouncePlant_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void BouncePlant_EditorDraw(void);
void BouncePlant_EditorLoad(void);
#endif
void BouncePlant_Serialize(void);

// Extra Entity Functions
void BoucePlant_SetupNodePositions(void);
int32 BoucePlant_GetNodeStandY(int32 x);
int32 BoucePlant_GetNodeY(int32 x);

#endif //!OBJ_BOUNCEPLANT_H
