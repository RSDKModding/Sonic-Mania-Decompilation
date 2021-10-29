#ifndef OBJ_UBERCATERKILLER_H
#define OBJ_UBERCATERKILLER_H

#include "SonicMania.h"

#define UberCaterkiller_SegmentCount (10)

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    bool32 defeated;
    uint16 sfxRocketJet;
    uint16 sfxHit;
    uint16 sfxExplosion2;
    uint16 sfxExplosion3;
    uint16 sfxSandSwim;
    uint16 sfxBumper3;
    uint16 sfxCaterJump;
    uint16 aniFrames;
} ObjectUberCaterkiller;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 invincibiltyTimer;
    int32 health;
    Vector2 bodyPositions[UberCaterkiller_SegmentCount];
    Vector2 bodyVelocity[UberCaterkiller_SegmentCount];
    int32 bodyTimers[UberCaterkiller_SegmentCount];
    int32 bodyScales[UberCaterkiller_SegmentCount];
    int32 bodyUnknown[UberCaterkiller_SegmentCount];
    int32 bodyAngles[UberCaterkiller_SegmentCount];
    Animator *bodyAnimators[UberCaterkiller_SegmentCount];
    uint8 aniID;
    uint8 unknownValue;
    int32 unknownValue2;
    int32 bodyStartX;
    uint8 field_1DC[PLAYER_MAX];
    Animator animator1;
    Animator animator2;
} EntityUberCaterkiller;

// Object Struct
extern ObjectUberCaterkiller *UberCaterkiller;

// Standard Entity Events
void UberCaterkiller_Update(void);
void UberCaterkiller_LateUpdate(void);
void UberCaterkiller_StaticUpdate(void);
void UberCaterkiller_Draw(void);
void UberCaterkiller_Create(void* data);
void UberCaterkiller_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void UberCaterkiller_EditorDraw(void);
void UberCaterkiller_EditorLoad(void);
#endif
void UberCaterkiller_Serialize(void);

// Extra Entity Functions
void UberCaterkiller_DebugSpawn(void);
void UberCaterkiller_DebugDraw(void);

void UberCaterkiller_CheckPlayerCollisions(void);
void UberCaterkiller_Explode(void);
void UberCaterkiller_Unknown5(int id);
void UberCaterkiller_SetupUnknown(int x, int y);

void UberCaterkiller_State_SetupArena(void);
void UberCaterkiller_SetupBodySizes(void);

void UberCaterkiller_State_Unknown1(void);
void UberCaterkiller_State_Unknown2(void);
void UberCaterkiller_State_Unknown3(void);
void UberCaterkiller_State_Unknown4(void);
void UberCaterkiller_State_Unknown5(void);
void UberCaterkiller_State_Unknown6(void);
void UberCaterkiller_State_Unknown7(void);
void UberCaterkiller_State_Destroyed(void);
void UberCaterkiller_State_Finish(void);

#endif //!OBJ_UBERCATERKILLER_H
