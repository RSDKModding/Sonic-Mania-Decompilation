#ifndef OBJ_TARGETBUMPER_H
#define OBJ_TARGETBUMPER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectTargetBumper;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 type;
    int32 hitCount;
    Vector2 startPos;
    Vector2 curPos;
    int32 field_74;
    Animator animator;
} EntityTargetBumper;

// Object Entity
extern ObjectTargetBumper *TargetBumper;

// Standard Entity Events
void TargetBumper_Update(void);
void TargetBumper_LateUpdate(void);
void TargetBumper_StaticUpdate(void);
void TargetBumper_Draw(void);
void TargetBumper_Create(void* data);
void TargetBumper_StageLoad(void);
void TargetBumper_EditorDraw(void);
void TargetBumper_EditorLoad(void);
void TargetBumper_Serialize(void);

// Extra Entity Functions
void TargetBumper_DebugSpawn(void);
void TargetBumper_DebugDraw(void);

void TargetBumper_Unknown3(void);
void TargetBumper_Unknown4(void);
void TargetBumper_Unknown5(void);

#endif //!OBJ_TARGETBUMPER_H
