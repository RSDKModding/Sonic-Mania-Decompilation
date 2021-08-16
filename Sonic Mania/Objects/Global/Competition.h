#ifndef OBJ_COMPETITION_H
#define OBJ_COMPETITION_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    int field_4;
    int field_8;
    int field_C;
    int field_10;
    int field_14;
    int field_18;
    int field_1C;
    int field_20;
    ushort aniFrames;
    Entity *activeEntity;
    int field_2C;
} ObjectCompetition;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    bool32 playerFlags[4];
    int timer;
    int seconds;
    int field_74;
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
void Competition_EditorDraw(void);
void Competition_EditorLoad(void);
void Competition_Serialize(void);

// Extra Entity Functions
void Competition_State_Manager(void);

void Competition_ResetOptions(void);
void Competition_ClearMatchData(void);
void Competition_CalculateScore(int playerID, byte flags);
#endif

#endif //!OBJ_COMPETITION_H
