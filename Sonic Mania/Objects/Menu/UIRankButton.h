#ifndef OBJ_UIRANKBUTTON_H
#define OBJ_UIRANKBUTTON_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectUIRankButton;

// Entity Class
typedef struct {
    MANIA_UI_ITEM_BASE
    bool32 showsName;
    LeaderboardEntry *leaderboardEntry;
    TextInfo rankText;
    TextInfo nameTimeText;
    LeaderboardEntry *prevLeaderboardEntry;
    int prevLeaderboardEntryStatus;
    int field_124;
    int field_128;
    int dword12C;
    int rank;
    int score;
    int row;
    int dword13C;
    int dword140;
    int dword144;
    int field_148;
    int field_14C;
    int field_150;
    int field_154;
    int dword158;
    int gap15C;
    int field_160;
    int field_164;
    int field_168;
    int field_16C;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    uint16 textSpriteIndex;
} EntityUIRankButton;

// Object Struct
extern ObjectUIRankButton *UIRankButton;

// Standard Entity Events
void UIRankButton_Update(void);
void UIRankButton_LateUpdate(void);
void UIRankButton_StaticUpdate(void);
void UIRankButton_Draw(void);
void UIRankButton_Create(void* data);
void UIRankButton_StageLoad(void);
void UIRankButton_EditorDraw(void);
void UIRankButton_EditorLoad(void);
void UIRankButton_Serialize(void);

// Extra Entity Functions
void UIRankButton_SetRankText(EntityUIRankButton *button, int rank);
void UIRankButton_Unknown2(int rank, EntityUIRankButton *button, int score, int row);
void UIRankButton_SetupRank(EntityUIRankButton *rankButton, LeaderboardEntry *entry);
void UIRankButton_Unknown4(void);
void UIRankButton_Unknown5(int colour, int a2, int a3, int a4, int a5);
bool32 UIRankButton_Options7CB(void);
bool32 UIRankButton_Options8CB(void);
void UIRankButton_Options5CB(void);
void UIRankButton_Options6CB(void);
void UIRankButton_FailCB(void);
void UIRankButton_Options3CB(void);
void UIRankButton_Unknown12(void);
void UIRankButton_Unknown13(void);
void UIRankButton_Unknown14(void);

#endif


#endif //!OBJ_UIRANKBUTTON_H
