#ifndef OBJ_DBTOWER_H
#define OBJ_DBTOWER_H

#include "SonicMania.h"

#define DBTower_SegmentCount (4)

// Object Class
struct ObjectDBTower {
    RSDK_OBJECT
    Hitbox hitboxSegment;
    bool32 defeated;
    uint16 sfxRocketJet;
    uint16 sfxHit;
    uint16 sfxExplosion2;
    uint16 sfxExplosion3;
    uint16 sfxBumper3;
    uint16 sfxAssemble;
    uint16 sfxRocketJet2;
    uint16 sfxRockemSockem;
    uint16 aniFrames;
};

// Entity Class
struct EntityDBTower {
    RSDK_ENTITY
    StateMachine(state);
    int32 timer;
    int32 invincibilityTimer;
    int32 health;
    Vector2 originPos;
    Vector2 bodyPositions[DBTower_SegmentCount];
    int32 segmentUnused1[DBTower_SegmentCount];
    int32 segmentUnused2[DBTower_SegmentCount];
    int32 segmentUnused3[DBTower_SegmentCount];
    int32 bodyAngles[DBTower_SegmentCount];
    int32 wobbleAngle;
    int32 wobbleAngleVel;
    int32 segmentOffsetY;
    int32 xOffsetAngle;
    uint8 connectedSegmentCount;
    Animator *segmentAnimators[DBTower_SegmentCount];
    uint8 playerTimers[PLAYER_MAX];
    Animator headAnimator;
    Animator bodyAnimator;
};

// Object Struct
extern ObjectDBTower *DBTower;

// Standard Entity Events
void DBTower_Update(void);
void DBTower_LateUpdate(void);
void DBTower_StaticUpdate(void);
void DBTower_Draw(void);
void DBTower_Create(void* data);
void DBTower_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void DBTower_EditorDraw(void);
void DBTower_EditorLoad(void);
#endif
void DBTower_Serialize(void);

// Extra Entity Functions
void DBTower_CheckPlayerCollisions_Head(void);
void DBTower_Explode(void);

void DBTower_State_SetupArena(void);
void DBTower_State_Setup_Encore(void);
void DBTower_State_HandleBoss(void);
void DBTower_State_Destroyed(void);
void DBTower_State_Finish(void);
#if MANIA_USE_PLUS
void DBTower_State_SpawnSignPost(void);
#endif

void DBTower_CheckPlayerCollisions_Body(void);
void DBTower_State_BodyBouncing(void);
void DBTower_State_BodyRolling(void);

#endif //!OBJ_DBTOWER_H
