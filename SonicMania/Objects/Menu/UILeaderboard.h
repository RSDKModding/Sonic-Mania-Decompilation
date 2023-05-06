#ifndef OBJ_UILEADERBOARD_H
#define OBJ_UILEADERBOARD_H

#include "Game.h"

// Object Class
struct ObjectUILeaderboard {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityUILeaderboard {
    MANIA_UI_ITEM_BASE
    int32 unused;
    LeaderboardEntry *leaderboardEntry;
    String unusedText[5];
    String rankText[5];
    String nameText[5];
    int32 times[5];
    int32 ranks[5];
    bool32 isUser[5];
    uint8 characterID;
    uint8 zoneID;
    uint8 actID;
    int32 zoneIconSprX;
    uint8 fuzzDir;
    bool32 lostFocus;
    int32 entryOffset;
    int32 entryLength;
#if !MANIA_USE_PLUS
    bool32 viewingUserRank;
    int32 taRecord;
#endif
    bool32 yPressCBEnabled;
    StateMachine(yPressCB);
    String zoneName;
    Animator unusedAnimator1;
    Animator unusedAnimator2;
    Animator zoneIconAnimator;
    Animator zoneNameAnimator;
    Animator textAnimator;
    Animator fuzzAnimator;
    Animator playerAnimator;
    Animator playerShadowAnimator;
    Animator playerNameAnimator;
    Animator actNumAnimator;
    Animator taAnimator;
    Animator timeAnimator;
    uint16 textFrames;
};

// Object Struct
extern ObjectUILeaderboard *UILeaderboard;

// Standard Entity Events
void UILeaderboard_Update(void);
void UILeaderboard_LateUpdate(void);
void UILeaderboard_StaticUpdate(void);
void UILeaderboard_Draw(void);
void UILeaderboard_Create(void *data);
void UILeaderboard_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UILeaderboard_EditorDraw(void);
void UILeaderboard_EditorLoad(void);
#endif
void UILeaderboard_Serialize(void);

// Extra Entity Functions
void UILeaderboard_SetupEntrySprites(EntityUILeaderboard *leaderboard);
#if !MANIA_USE_PLUS
void UILeaderboard_InitLeaderboard(EntityUILeaderboard *leaderboard);
void UILeaderboard_SetupLeaderboard(EntityUILeaderboard *leaderboard, uint8 character, uint8 zone, uint8 act);
#endif
void UILeaderboard_LoadEntries(EntityUILeaderboard *entity);

void UILeaderboard_DrawPrimitives(void);
void UILeaderboard_DrawEntries(void);
void UILeaderboard_DrawZonePreview(void);
void UILeaderboard_DrawTime(int32 mins, int32 secs, int32 millisecs, int32 x, int32 y);
void UILeaderboard_DrawRank(int32 id);

void UILeaderboard_State_Init(void);
void UILeaderboard_State_Unselected(void);
void UILeaderboard_State_Selected(void);

void UILeaderboard_ProcessButtonCB(void);

#endif //! OBJ_UILEADERBOARD_H
