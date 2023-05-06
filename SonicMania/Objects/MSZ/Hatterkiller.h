#ifndef OBJ_HATTERKILLER_H
#define OBJ_HATTERKILLER_H

#include "Game.h"

#define HATTERKILLER_SEGMENT_COUNT (10)

// Object Class
struct ObjectHatterkiller {
    RSDK_OBJECT
    Hitbox hitboxSegment;
    uint16 sfxRocketJet;
    uint16 sfxTransform2;
    uint16 sfxPowerup;
    uint16 aniFrames;
};

// Entity Class
struct EntityHatterkiller {
    RSDK_ENTITY
    int32 timer;
    Vector2 bodyPositions[HATTERKILLER_SEGMENT_COUNT];
    Vector2 bodyVelocities[HATTERKILLER_SEGMENT_COUNT];
    int32 bodyDelays[HATTERKILLER_SEGMENT_COUNT];
    Animator *bodyAnimators[HATTERKILLER_SEGMENT_COUNT];
    Animator headAnimator;
    Animator bodyAnimator;
    Animator tailAnimator;
};

// Object Struct
extern ObjectHatterkiller *Hatterkiller;

// Standard Entity Events
void Hatterkiller_Update(void);
void Hatterkiller_LateUpdate(void);
void Hatterkiller_StaticUpdate(void);
void Hatterkiller_Draw(void);
void Hatterkiller_Create(void *data);
void Hatterkiller_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Hatterkiller_EditorDraw(void);
void Hatterkiller_EditorLoad(void);
#endif
void Hatterkiller_Serialize(void);

// Extra Entity Functions
void Hatterkiller_DebugDraw(void);
void Hatterkiller_DebugSpawn(void);

#endif //! OBJ_HATTERKILLER_H
