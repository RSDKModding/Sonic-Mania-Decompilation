#ifndef OBJ_RATTLEKILLER_H
#define OBJ_RATTLEKILLER_H

#include "Game.h"

#define RATTLEKILLER_SEGMENT_COUNT (10)

typedef enum {
    RATTLEKILLER_IDLE,
    RATTLEKILLER_TWIST_HORIZONTAL,
    RATTLEKILLER_TWIST_DOWN,
    RATTLEKILLER_TWIST_UP,
    RATTLEKILLER_PLAYERDETECTED,
    RATTLEKILLER_STRETCHBODY,
    RATTLEKILLER_EXTEND,
    RATTLEKILLER_RETRACT,
} RattlekillerBodyStates;

// Object Class
struct ObjectRattlekiller {
    RSDK_OBJECT
    Hitbox hitboxSegment;
    uint16 sfxRocketJet;
    uint16 aniFrames;
};

// Entity Class
struct EntityRattlekiller {
    RSDK_ENTITY
    StateMachine(state);     // Unused
    StateMachine(stateDraw); // Unused
    int32 timer;
    int32 bodyStates[RATTLEKILLER_SEGMENT_COUNT];
    Vector2 bodyPositions[RATTLEKILLER_SEGMENT_COUNT];
    Vector2 bodyOriginPos[RATTLEKILLER_SEGMENT_COUNT];
    Vector2 bodyVelocities[RATTLEKILLER_SEGMENT_COUNT];
    int32 bodyAngles[RATTLEKILLER_SEGMENT_COUNT];
    int32 bodyDepth[RATTLEKILLER_SEGMENT_COUNT];
    int32 bodyDelays[RATTLEKILLER_SEGMENT_COUNT];
    int32 bodyIDs[RATTLEKILLER_SEGMENT_COUNT];
    Animator *bodyAnimators[RATTLEKILLER_SEGMENT_COUNT];
    Vector2 topBounds;
    Vector2 bottomBounds;
    Vector2 targetPos;
    int32 length;
    Animator headAnimator;
    Animator bodyAnimator;
    Animator tailAnimator;
};

// Object Struct
extern ObjectRattlekiller *Rattlekiller;

// Standard Entity Events
void Rattlekiller_Update(void);
void Rattlekiller_LateUpdate(void);
void Rattlekiller_StaticUpdate(void);
void Rattlekiller_Draw(void);
void Rattlekiller_Create(void *data);
void Rattlekiller_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Rattlekiller_EditorDraw(void);
void Rattlekiller_EditorLoad(void);
#endif
void Rattlekiller_Serialize(void);

// Extra Entity Functions
void Rattlekiller_DebugSpawn(void);
void Rattlekiller_DebugDraw(void);

void Rattlekiller_HandleSorting(void);

#endif //! OBJ_RATTLEKILLER_H
