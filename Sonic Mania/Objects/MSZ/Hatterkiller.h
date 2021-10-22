#ifndef OBJ_HATTERKILLER_H
#define OBJ_HATTERKILLER_H

#include "SonicMania.h"

#define Hatterkiller_SegmentCount (10)

// Object Class
typedef struct {
    RSDK_OBJECT
    Hitbox hitbox;
    uint16 sfxRocketJet;
    uint16 sfxTransform2;
    uint16 sfxPowerup;
    uint16 aniFrames;
} ObjectHatterkiller;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int field_58;
    Vector2 positions[Hatterkiller_SegmentCount];
    Vector2 velocities[Hatterkiller_SegmentCount];
    int delays[Hatterkiller_SegmentCount];
    Animator *animators[Hatterkiller_SegmentCount];
    Animator animator1;
    Animator animator2;
    Animator animator3;
} EntityHatterkiller;

// Object Struct
extern ObjectHatterkiller *Hatterkiller;

// Standard Entity Events
void Hatterkiller_Update(void);
void Hatterkiller_LateUpdate(void);
void Hatterkiller_StaticUpdate(void);
void Hatterkiller_Draw(void);
void Hatterkiller_Create(void* data);
void Hatterkiller_StageLoad(void);
void Hatterkiller_EditorDraw(void);
void Hatterkiller_EditorLoad(void);
void Hatterkiller_Serialize(void);

// Extra Entity Functions
void Hatterkiller_DebugDraw(void);
void Hatterkiller_DebugSpawn(void);

#endif //!OBJ_HATTERKILLER_H
