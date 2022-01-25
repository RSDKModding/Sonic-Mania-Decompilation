#ifndef OBJ_RATTLEKILLER_H
#define OBJ_RATTLEKILLER_H

#include "SonicMania.h"

#define Rattlekiller_SegmentCount (10)

// Object Class
struct ObjectRattlekiller {
    RSDK_OBJECT
    Hitbox hitbox;
    uint16 sfxRocketJet;
    uint16 aniFrames;
};

// Entity Class
struct EntityRattlekiller {
    RSDK_ENTITY
    int32 field_58;
    int32 field_5C;
    int32 timer;
    int32 bodyStates[Rattlekiller_SegmentCount];
    Vector2 bodyPositions[Rattlekiller_SegmentCount];
    Vector2 field_DC[Rattlekiller_SegmentCount];
    Vector2 field_12C[Rattlekiller_SegmentCount];
    int32 bodyAngles[Rattlekiller_SegmentCount];
    int32 field_1A4[Rattlekiller_SegmentCount];
    int32 bodyDelays[Rattlekiller_SegmentCount];
    int32 bodyIDs[Rattlekiller_SegmentCount];
    Animator *bodyAnimators[Rattlekiller_SegmentCount];
    Vector2 startPos;
    Vector2 startPos2;
    Vector2 playerPos;
    int32 length;
    Animator animators[3];
};

// Object Struct
extern ObjectRattlekiller *Rattlekiller;

// Standard Entity Events
void Rattlekiller_Update(void);
void Rattlekiller_LateUpdate(void);
void Rattlekiller_StaticUpdate(void);
void Rattlekiller_Draw(void);
void Rattlekiller_Create(void* data);
void Rattlekiller_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Rattlekiller_EditorDraw(void);
void Rattlekiller_EditorLoad(void);
#endif
void Rattlekiller_Serialize(void);

// Extra Entity Functions
void Rattlekiller_DebugSpawn(void);
void Rattlekiller_DebugDraw(void);

void Rattlekiller_HandleSorting(void);

#endif //!OBJ_RATTLEKILLER_H
