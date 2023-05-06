#ifndef OBJ_TARGETBUMPER_H
#define OBJ_TARGETBUMPER_H

#include "Game.h"

typedef enum { TARGETBUMP_HORIZONTAL, TARGETBUMP_VERTICAL, TARGETBUMP_DIAGONAL } TargetBumperTypes;

typedef enum { TARGETBUMP_THREE_HIT, TARGETBUMP_TWO_HIT, TARGETBUMP_ONE_HIT } TargetBumperHitCount;

// Object Class
struct ObjectTargetBumper {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityTargetBumper {
    RSDK_ENTITY
    StateMachine(state);
    int32 type;
    int32 hitCount;
    Vector2 startPos;
    Vector2 curPos;
    int32 hitTimer;
    Animator animator;
};

// Object Entity
extern ObjectTargetBumper *TargetBumper;

// Standard Entity Events
void TargetBumper_Update(void);
void TargetBumper_LateUpdate(void);
void TargetBumper_StaticUpdate(void);
void TargetBumper_Draw(void);
void TargetBumper_Create(void *data);
void TargetBumper_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void TargetBumper_EditorDraw(void);
void TargetBumper_EditorLoad(void);
#endif
void TargetBumper_Serialize(void);

// Extra Entity Functions
void TargetBumper_DebugSpawn(void);
void TargetBumper_DebugDraw(void);

void TargetBumper_CheckPlayerCollisions(void);

void TargetBumper_State_Idle(void);
void TargetBumper_State_Hit(void);

#endif //! OBJ_TARGETBUMPER_H
