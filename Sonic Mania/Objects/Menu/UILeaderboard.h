#ifndef OBJ_UILEADERBOARD_H
#define OBJ_UILEADERBOARD_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort aniFrames;
} ObjectUILeaderboard;

// Entity Class
typedef struct {
    MANIA_UI_ITEM_BASE
    int field_104;
    int field_108;
    TextInfo text1[5];
    TextInfo rankText[5];
    TextInfo nameText[5];
    int times[5];
    int ranks[5];
    int isUser[5];
    byte frameID2;
    byte frameID;
    byte frameID3;
    int timer2;
    byte editorFlip;
    bool32 field_1CC;
    int entryOffset;
    int field_1D4;
    int field_1D8;
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
    ushort textSpriteIndex;
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
void UILeaderboard_EditorDraw(void);
void UILeaderboard_EditorLoad(void);
void UILeaderboard_Serialize(void);

// Extra Entity Functions
void UILeaderboard_SetupEntrySprites(EntityUILeaderboard *entity);
void UILeaderboard_LoadEntries(EntityUILeaderboard *entity);

void UILeaderboard_DrawPrimitives(void);
void UILeaderboard_DrawEntries(void);
void UILeaderboard_Unknown3(void);
void UILeaderboard_DrawTime(int mins, int secs, int millisecs, int x, int y);
void UILeaderboard_DrawRank(int id);

void UILeaderboard_State_Unknown1(void);
void UILeaderboard_State_Unknown2(void);
void UILeaderboard_State_Unknown3(void);

void UILeaderboard_ProcessButtonCB(void);

#endif //!OBJ_UILEADERBOARD_H
