#ifndef OBJ_COMPETITION_H
#define OBJ_COMPETITION_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    int32 field_4;
    int32 field_8;
    int32 field_C;
    int32 field_10;
    int32 field_14;
    int32 field_18;
    int32 field_1C;
    int32 field_20;
    uint16 aniFrames;
    Entity *activeEntity;
    int32 field_2C;
} ObjectCompetition;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    bool32 playerFlags[4];
    int32 timer;
    int32 seconds;
    int32 field_74;
    Animator animator;
} EntityCompetition;

// Object Struct
extern ObjectCompetition *Competition;

// Standard Entity Events
void Competition_Update(void);
void Competition_LateUpdate(void);
void Competition_StaticUpdate(void);
void Competition_Draw(void);
void Competition_Create(void* data);
void Competition_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Competition_EditorDraw(void);
void Competition_EditorLoad(void);
#endif
void Competition_Serialize(void);

// Extra Entity Functions
void Competition_State_Manager(void);

void Competition_ResetOptions(void);
void Competition_ClearMatchData(void);
void Competition_CalculateScore(int32 playerID, uint8 flags);
#endif

#endif //!OBJ_COMPETITION_H
