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
    int32 prevLeaderboardEntryStatus;
    int32 field_124;
    int32 field_128;
    int32 dword12C;
    int32 rank;
    int32 score;
    int32 row;
    int32 dword13C;
    int32 dword140;
    int32 dword144;
    int32 field_148;
    int32 field_14C;
    int32 field_150;
    int32 field_154;
    int32 dword158;
    int32 gap15C;
    int32 field_160;
    int32 field_164;
    int32 field_168;
    int32 field_16C;
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
#if RETRO_INCLUDE_EDITOR
void UIRankButton_EditorDraw(void);
void UIRankButton_EditorLoad(void);
#endif
void UIRankButton_Serialize(void);

// Extra Entity Functions
void UIRankButton_SetRankText(EntityUIRankButton *button, int32 rank);
void UIRankButton_Unknown2(int32 rank, EntityUIRankButton *button, int32 score, int32 row);
void UIRankButton_SetupRank(EntityUIRankButton *rankButton, LeaderboardEntry *entry);
void UIRankButton_Unknown4(void);
void UIRankButton_Unknown5(int32 colour, int32 a2, int32 a3, int32 a4, int32 a5);
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
