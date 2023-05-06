#ifndef OBJ_UIREPLAYCAROUSEL_H
#define OBJ_UIREPLAYCAROUSEL_H

#include "Game.h"

#if MANIA_USE_PLUS
// Object Class
struct ObjectUIReplayCarousel {
    RSDK_OBJECT
    uint16 aniFrames;
    EntityUIButtonPrompt *prompt;
};

// Entity Class
struct EntityUIReplayCarousel {
    MANIA_UI_ITEM_BASE
    String zoneNameText[4];
    String createdAtText[4];
    String loadingText;
    String noReplaysText;
    int32 sprX;
    uint8 fuzzDirection;
    int32 unused1;
    StateMachine(stateDraw);
    int16 curReplayID;
    uint16 sortedRowCount;
    bool32 isMoving;
    int32 curViewOffset;
    int32 targetViewOffset;
    uint8 visibleReplayCount;
    uint8 visibleReplayOffset;
    int32 unused2;
    int32 popoverPos;
    uint8 prevReplayOffset;
    Animator unusedAnimator1;
    Animator zoneIconAnimator;
    Animator fuzzAnimator;
    Animator detailsAnimator;
    Animator unusedAnimator2;
    Animator zoneNameAnimators[4];
    Animator unusedAnimator3;
    Animator unusedAnimator4;
    Animator unusedAnimator5;
    Animator unusedAnimator6;
    Animator createTimeAnimator;
    int32 language;
};

// Object Struct
extern ObjectUIReplayCarousel *UIReplayCarousel;

// Standard Entity Events
void UIReplayCarousel_Update(void);
void UIReplayCarousel_LateUpdate(void);
void UIReplayCarousel_StaticUpdate(void);
void UIReplayCarousel_Draw(void);
void UIReplayCarousel_Create(void *data);
void UIReplayCarousel_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UIReplayCarousel_EditorDraw(void);
void UIReplayCarousel_EditorLoad(void);
#endif
void UIReplayCarousel_Serialize(void);

// Extra Entity Functions
void UIReplayCarousel_ProcessButtonCB(void);
void UIReplayCarousel_HandlePositions(void);
void UIReplayCarousel_SetupButtonCallbacks(void);
void UIReplayCarousel_HandleTouchPositions(void);
void UIReplayCarousel_TouchedCB(void);
void UIReplayCarousel_SetupVisibleReplayButtons(void);
void UIReplayCarousel_DrawBGShapes(int32 drawX, int32 drawY, bool32 encore, int16 replayID);
void UIReplayCarousel_DrawStageInfo(int32 drawX, int32 drawY, uint8 act, uint8 characterID, int32 score, int32 replayID);
void UIReplayCarousel_DrawZoneIcon(int32 drawX, int32 drawY, uint8 zoneID, int16 replayID);
void UIReplayCarousel_DrawReplayInfo(int32 drawX, int32 drawY, uint8 zoneID, uint8 act, uint8 characterID, bool32 isEncore, int32 score,
                                     int32 replayID);
void UIReplayCarousel_StartAction(void);
void UIReplayCarousel_SetUnselected(void);

void UIReplayCarousel_Draw_Loading(void);
void UIReplayCarousel_Draw_NoReplays(void);
void UIReplayCarousel_Draw_Carousel(void);

void UIReplayCarousel_State_Init(void);
void UIReplayCarousel_State_Unselected(void);
void UIReplayCarousel_State_Selected(void);
void UIReplayCarousel_State_StartAction(void);

#endif

#endif //! OBJ_UIREPLAYCAROUSEL_H
