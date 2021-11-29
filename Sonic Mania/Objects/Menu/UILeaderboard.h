#ifndef OBJ_UILEADERBOARD_H
#define OBJ_UILEADERBOARD_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectUILeaderboard;

// Entity Class
typedef struct {
    MANIA_UI_ITEM_BASE
    int32 field_104;
    LeaderboardEntry *leaderboardEntry;
    TextInfo text1[5];
    TextInfo rankText[5];
    TextInfo nameText[5];
    int32 times[5];
    int32 ranks[5];
    int32 isUser[5];
    uint8 playerID;
    uint8 zoneID;
    uint8 actID;
    int32 timer2;
    uint8 editorFlip;
    bool32 field_1CC;
    int32 entryOffset;
    int32 entryLength;
#if !RETRO_USE_PLUS
    int32 entryIsUser;
    int32 taRecord;
#endif
    int32 field_1D8;
    StateMachine(yPressCB);
    TextInfo field_1E0;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    Animator animator5;
    Animator animator6;
    Animator animator7;
    Animator animator8;
    Animator animator9;
    Animator animator10;
    Animator animator11;
    Animator animator12;
    uint16 textFrames;
} EntityUILeaderboard;

// Object Struct
extern ObjectUILeaderboard *UILeaderboard;

// Standard Entity Events
void UILeaderboard_Update(void);
void UILeaderboard_LateUpdate(void);
void UILeaderboard_StaticUpdate(void);
void UILeaderboard_Draw(void);
void UILeaderboard_Create(void* data);
void UILeaderboard_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void UILeaderboard_EditorDraw(void);
void UILeaderboard_EditorLoad(void);
#endif
void UILeaderboard_Serialize(void);

// Extra Entity Functions
void UILeaderboard_SetupEntrySprites(EntityUILeaderboard *entity);
#if !RETRO_USE_PLUS 
void UILeaderboard_InitLeaderboard(EntityUILeaderboard *leaderboard);
void UILeaderboard_SetupLeaderboard(EntityUILeaderboard *leaderboard, uint8 player, uint8 zone, uint8 act);
#endif
void UILeaderboard_LoadEntries(EntityUILeaderboard *entity);

void UILeaderboard_DrawPrimitives(void);
void UILeaderboard_DrawEntries(void);
void UILeaderboard_Unknown3(void);
void UILeaderboard_DrawTime(int32 mins, int32 secs, int32 millisecs, int32 x, int32 y);
void UILeaderboard_DrawRank(int32 id);

void UILeaderboard_State_Unknown1(void);
void UILeaderboard_State_Unknown2(void);
void UILeaderboard_State_Unknown3(void);

void UILeaderboard_ProcessButtonCB(void);

#endif //!OBJ_UILEADERBOARD_H
