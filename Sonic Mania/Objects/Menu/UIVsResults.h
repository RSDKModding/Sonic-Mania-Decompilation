#ifndef OBJ_UIVSRESULTS_H
#define OBJ_UIVSRESULTS_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort aniFrames;
    ushort textFrames;
} ObjectUIVsResults;

// Entity Class
typedef struct {
    MANIA_UI_ITEM_BASE
    byte playerID;
    int numRows;
    int trophyCount;
    byte row0Label;
    byte row1Label;
    byte row2Label;
    byte row3Label;
    byte row4Label;
    byte row5Label;
    byte row6Label;
    byte row7Label;
    byte row8Label;
    byte row9Label;
    byte row10Label;
    byte row11Label;
    byte row12Label;
    bool32 row0Highlight;
    bool32 row1Highlight;
    bool32 row2Highlight;
    bool32 row3Highlight;
    bool32 row4Highlight;
    bool32 row5Highlight;
    bool32 row6Highlight;
    bool32 row7Highlight;
    bool32 row8Highlight;
    bool32 row9Highlight;
    bool32 row10Highlight;
    bool32 row11Highlight;
    bool32 row12Highlight;
    TextInfo rowText[13];
    int characterID;
    int field_1C0;
    int offset;
    int numRowStore;
    int field_1CC;
    int field_1D0;
    int field_1D4;
    int field_1D8;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator textAnimator;
    Animator animator4;
    Animator rowAnimators[13];
    Animator animator5;
    ushort textSpriteIndex;
} EntityUIVsResults;

// Object Struct
extern ObjectUIVsResults *UIVsResults;

// Standard Entity Events
void UIVsResults_Update(void);
void UIVsResults_LateUpdate(void);
void UIVsResults_StaticUpdate(void);
void UIVsResults_Draw(void);
void UIVsResults_Create(void* data);
void UIVsResults_StageLoad(void);
void UIVsResults_EditorDraw(void);
void UIVsResults_EditorLoad(void);
void UIVsResults_Serialize(void);

// Extra Entity Functions
void UIVsResults_SetupSprites(void);
void UIVsResults_Unknown2(void);
void UIVsResults_Unknown3(void);
void UIVsResults_DrawRow(int row, int posX, int posY);
void UIVsResults_Unknown5(void);
void UIVsResults_Unknown6(void);
void UIVsResults_Unknown7(void);

#endif //!OBJ_UIVSRESULTS_H
