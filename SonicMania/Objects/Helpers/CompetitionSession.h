#ifndef OBJ_COMPETITIONSESSION_H
#define OBJ_COMPETITIONSESSION_H

#include "Game.h"

#if !MANIA_USE_PLUS
// preplus was always 2P
#define COMPETITION_PLAYER_COUNT (2)

#define CompSession_DeriveWinner CompetitionSession_DeriveWinner
#else

#define CompSession_DeriveWinner Competition_DeriveWinner
#endif

#define COMPETITION_STAGE_COUNT (12)

typedef enum {
    FINISHTYPE_NOTFINISHED,
    FINISHTYPE_GAMEOVER,
    FINISHTYPE_PASSEDSIGNPOST,
} vsFinishFlags;

typedef enum {
    VS_BORDER_NONE,
    VS_BORDER_BOTTOMRIGHT,
    VS_BORDER_BOTTOM,
    VS_BORDER_BOTTOMLEFT,
    VS_BORDER_TOPRIGHT,
    VS_BORDER_TOP,
    VS_BORDER_TOPLEFT,
} vsScreenBorderTypes;

// created so I can easily set up times in an array
typedef struct {
    int32 minutes;
    int32 seconds;
    int32 milliseconds;
} vsTime;

// Object Class
struct ObjectCompetitionSession {
    RSDK_OBJECT
};

// Entity Class
struct EntityCompetitionSession {
    RSDK_ENTITY
    bool32 inMatch;
    int32 playerCount;
    int32 stageIndex;
    int32 zoneID;
    int32 actID;
    int32 matchID;
    int32 matchCount;
    int32 itemMode;
#if MANIA_USE_PLUS
    int32 swapType;
#endif
    bool32 completedStages[COMPETITION_STAGE_COUNT];
    uint8 playerID[PLAYER_COUNT];
    uint8 matchWinner[COMPETITION_STAGE_COUNT];
    int32 rings[PLAYER_COUNT];
    int32 score[PLAYER_COUNT];
    int32 items[PLAYER_COUNT];
    vsTime time[PLAYER_COUNT];
    uint8 finishState[PLAYER_COUNT];
    int32 totalRings[PLAYER_COUNT];
    int32 wins[PLAYER_COUNT];
    int32 lives[PLAYER_COUNT];
#if MANIA_USE_PLUS
    int32 screenBorderType[SCREEN_COUNT];
    int32 displayMode;
    int32 inputSlots[PLAYER_COUNT];
    int32 prevMatchID;
#endif
};

// Object Entity
extern ObjectCompetitionSession *CompetitionSession;

// Standard Entity Events
void CompetitionSession_Update(void);
void CompetitionSession_LateUpdate(void);
void CompetitionSession_StaticUpdate(void);
void CompetitionSession_Draw(void);
void CompetitionSession_Create(void *data);
void CompetitionSession_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void CompetitionSession_EditorDraw(void);
void CompetitionSession_EditorLoad(void);
#endif
void CompetitionSession_Serialize(void);

// Extra Entity Functions
EntityCompetitionSession *CompetitionSession_GetSession(void);

#if !MANIA_USE_PLUS
void CompetitionSession_ResetOptions(void);
void CompetitionSession_ClearMatchData(void);
void CompetitionSession_DeriveWinner(int32 playerID, int32 finishType);
void CompetitionSession_WinMatchFor(int32 playerID);
#endif

#endif //! OBJ_COMPETITIONSESSION_H
