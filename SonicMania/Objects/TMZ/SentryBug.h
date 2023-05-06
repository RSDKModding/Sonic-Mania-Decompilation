#ifndef OBJ_SENTRYBUG_H
#define OBJ_SENTRYBUG_H

#include "Game.h"

// Object Class
struct ObjectSentryBug {
    RSDK_OBJECT
    TABLE(int32 orbOffsets_Attatched[84], { -0x50000, 0x30000,  -0x90000, 0xA0000,  -0x70000, 0x100000, 0x70000,  0x30000,  0x30000,  0xA0000,
                                            -0x30000, 0x100000, -0x70000, 0x30000,  -0xB0000, 0xA0000,  -0x80000, 0x100000, 0x90000,  0x30000,
                                            0x60000,  0xA0000,  0,        0x100000, -0x90000, 0x30000,  -0xB0000, 0xA0000,  -0x80000, 0x100000,
                                            0x90000,  0x30000,  0x80000,  0xA0000,  0x30000,  0x100000, -0x90000, 0x30000,  -0xB0000, 0xA0000,
                                            -0x80000, 0x100000, 0x90000,  0x30000,  0xB0000,  0xA0000,  0x80000,  0x100000, 0x90000,  0x30000,
                                            0xB0000,  0xA0000,  0x80000,  0x100000, -0x90000, 0x30000,  -0x80000, 0xA0000,  -0x30000, 0x100000,
                                            0x70000,  0x30000,  0xB0000,  0xA0000,  0x80000,  0x100000, -0x90000, 0x30000,  -0x60000, 0xA0000,
                                            0,        0x100000, 0x50000,  0x30000,  0x90000,  0xA0000,  0x70000,  0x100000, -0x70000, 0x30000,
                                            -0x30000, 0xA0000,  0x30000,  0x100000 });
    TABLE(int32 dropVelR[12], { -0x10000, -0x10000, -0x20000, -0x8000, -0x10000, 0x8000, 0x10000, -0x10000, 0x20000, -0x8000, 0x10000, 0x8000 });
    TABLE(int32 dropVelL[12], { 0x10000, -0x10000, 0x20000, -0x8000, 0x10000, 0x8000, -0x10000, -0x10000, -0x20000, -0x8000, -0x10000, 0x8000 });
    Hitbox hitbox;
    uint16 meshFrames;
    uint16 sceneIndex;
    uint16 aniFrames;
    uint16 sfxPon;
    uint16 sfxSwarm;
};

// Entity Class
struct EntitySentryBug {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateOrbs);
    uint8 speed;
    Vector2 amplitude;
    bool32 orbsDetatched;
    bool32 showNet;
    int32 timer;
    Vector2 originPos;
    Vector2 startPos;
    EntityPlayer *playerPtr;
    Vector2 netPos;
    int32 netRotation;
    int32 netScale;
    int32 netColor;
    Vector2 orbPositions[6];
    Vector2 orbVelocities[6];
    Animator unusedAnimator;
    Matrix matUnused;
    Matrix matTransform;
    Matrix matWorld;
    Animator bugAnimator;
    Animator orbAnimator;
};

// Object Struct
extern ObjectSentryBug *SentryBug;

// Standard Entity Events
void SentryBug_Update(void);
void SentryBug_LateUpdate(void);
void SentryBug_StaticUpdate(void);
void SentryBug_Draw(void);
void SentryBug_Create(void *data);
void SentryBug_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void SentryBug_EditorDraw(void);
void SentryBug_EditorLoad(void);
#endif
void SentryBug_Serialize(void);

// Extra Entity Functions
void SentryBug_DebugSpawn(void);
void SentryBug_DebugDraw(void);

void SentryBug_CheckPlayerCollisions(void);

void SentryBug_SetupOrbDropVelocity(void);
void SentryBug_StateOrbs_Attached(void);
void SentryBug_StateOrbs_Dropped(void);
void SentryBug_StateOrbs_BeginNetRotation(void);
void SentryBug_StateOrbs_RotateAroundNet(void);
void SentryBug_StateOrbs_ReturnToSlots(void);

void SentryBug_State_AwaitPlayer(void);
void SentryBug_State_DropOrbs(void);
void SentryBug_State_NetAppear(void);
void SentryBug_State_NetShrink(void);
void SentryBug_State_NetFlash(void);
void SentryBug_State_ReturnToSlots(void);

#endif //! OBJ_SENTRYBUG_H
