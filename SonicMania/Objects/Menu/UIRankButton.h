#ifndef OBJ_UIRANKBUTTON_H
#define OBJ_UIRANKBUTTON_H

#include "Game.h"

#if MANIA_USE_PLUS
// Object Class
struct ObjectUIRankButton {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityUIRankButton {
    MANIA_UI_ITEM_BASE
    bool32 showsName;
    LeaderboardEntry *leaderboardEntry;
    String rankText;
    String nameTimeText;
    LeaderboardEntry *prevLeaderboardEntry;
    int32 prevLeaderboardEntryStatus;
    Vector2 popoverPos;
    bool32 hasChanged; // never actually set to true, but who knows!
    int32 rank;
    int32 score;
    int32 replayID;
    Vector2 size;
    int32 bgEdgeSize; // just a guess but maybe its right, its unused anyways
    int32 textBounceOffset;
    int32 buttonBounceOffset;
    int32 textBounceVelocity;
    int32 buttonBounceVelocity;
    bool32 textVisible;
    int32 unused1;
    int32 unused2;
    int32 unused3;
    int32 unused4;
    int32 unused5;
    Animator textAnimator; // unused, despite being set in Create
    Animator fontAnimator;
    Animator replayIconAnimator;
    Animator rankingAnimator;
    uint16 textFrames;
};

// Object Struct
extern ObjectUIRankButton *UIRankButton;

// Standard Entity Events
void UIRankButton_Update(void);
void UIRankButton_LateUpdate(void);
void UIRankButton_StaticUpdate(void);
void UIRankButton_Draw(void);
void UIRankButton_Create(void *data);
void UIRankButton_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UIRankButton_EditorDraw(void);
void UIRankButton_EditorLoad(void);
#endif
void UIRankButton_Serialize(void);

// Extra Entity Functions
void UIRankButton_SetRankText(EntityUIRankButton *button, int32 rank);
void UIRankButton_SetTimeAttackRank(EntityUIRankButton *button, int32 rank, int32 score, int32 replayID);
void UIRankButton_SetupLeaderboardRank(EntityUIRankButton *button, LeaderboardEntry *entry);

void UIRankButton_DrawSprites(void);
void UIRankButton_DrawBackgroundShape(int32 x, int32 y, int32 width, int32 height, int32 color);

bool32 UIRankButton_CheckButtonEnterCB(void);
bool32 UIRankButton_CheckSelectedCB(void);
void UIRankButton_ButtonEnterCB(void);
void UIRankButton_ButtonLeaveCB(void);
void UIRankButton_FailCB(void);
void UIRankButton_SelectedCB(void);

void UIRankButton_State_HandleButtonLeave(void);
void UIRankButton_State_HandleButtonEnter(void);
void UIRankButton_State_Selected(void);

#endif

#endif //! OBJ_UIRANKBUTTON_H
