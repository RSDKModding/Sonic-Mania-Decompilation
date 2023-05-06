#ifndef OBJ_UITAZONEMODULE_H
#define OBJ_UITAZONEMODULE_H

#include "Game.h"

// Object Class
struct ObjectUITAZoneModule {
    RSDK_OBJECT
    uint16 aniFrames;
    EntityUIButtonPrompt *leaderboardsPrompt;
#if MANIA_USE_PLUS
    bool32 isEncoreMode;
#endif
    bool32 showLBPrompt;
};

// Entity Class
struct EntityUITAZoneModule {
    MANIA_UI_ITEM_BASE
#if !MANIA_USE_PLUS
    bool32 isExpanding;
#endif
    uint8 zoneID;
    String text1;
    String text2;
#if !MANIA_USE_PLUS
    bool32 debugExpand;
#endif
    bool32 wasDisabled;
    String text1Store;
    String text2Store;
    int32 zoneIconSprX;
    Vector2 drawPos;
#if !MANIA_USE_PLUS
    int32 prevExpandAmount;
    int32 expandAmount;
    uint8 expandTimer;
    int32 announceTimer;
#endif
    uint8 fuzzDir;
    uint8 characterID;
    uint8 actID;
#if MANIA_USE_PLUS
    bool32 isEncore;
#else
    uint8 rank;
#endif
    int32 unused1;
    int32 unused2;
#if MANIA_USE_PLUS
    int32 unused3;
    int32 unused4;
#endif
#if !MANIA_USE_PLUS
    Animator textAnimator;
#endif
    Animator zoneIconsAnimator;
    Animator fuzzAnimator;
    Animator timeAttackAnimator;
    Animator medLetterAnimator1;
    Animator medLetterAnimator2;
};

// Object Struct
extern ObjectUITAZoneModule *UITAZoneModule;

// Standard Entity Events
void UITAZoneModule_Update(void);
void UITAZoneModule_LateUpdate(void);
void UITAZoneModule_StaticUpdate(void);
void UITAZoneModule_Draw(void);
void UITAZoneModule_Create(void *data);
void UITAZoneModule_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UITAZoneModule_EditorDraw(void);
void UITAZoneModule_EditorLoad(void);
#endif
void UITAZoneModule_Serialize(void);

// Extra Entity Functions
void UITAZoneModule_Setup(void);
void UITAZoneModule_SetupText(void);
void UITAZoneModule_DrawBGShapes(void);
void UITAZoneModule_DrawFGShapes(void);
void UITAZoneModule_DrawZonePreview(void);
void UITAZoneModule_DrawModuleInfo(void);
void UITAZoneModule_FailCB(void);
void UITAZoneModule_SelectedCB(void);
bool32 UITAZoneModule_CheckButtonEnterCB(void);
bool32 UITAZoneModule_CheckSelectedCB(void);
void UITAZoneModule_ButtonEnterCB(void);
void UITAZoneModule_ButtonLeaveCB(void);
void UITAZoneModule_TransitionCB(void);
void UITAZoneModule_ShowLeaderboards_CB(void);
void UITAZoneModule_State_Init(void);
void UITAZoneModule_State_NotSelected(void);
void UITAZoneModule_State_Selected(void);
void UITAZoneModule_State_HasBeenSelected(void);

#if !MANIA_USE_PLUS
Vector2 UITAZoneModule_DrawTime(int32 x, int32 y, int32 minutes, int32 seconds, int32 milliseconds);
void UITAZoneModule_DrawActInfo_Expanded(void);
void UITAZoneModule_DrawExpandedView(void);
void UITAZoneModule_State_Expanded(void);
void UITAZoneModule_State_StartTimeAttackAttempt(void);
void UITAZoneModule_TouchCB_Left(void);
void UITAZoneModule_TouchCB_Right(void);
void UITAZoneModule_ProcessButtonCB_Expanded(void);
void UITAZoneModule_ShowLeaderboards(int32 player, int32 zone, int32 act, bool32 wasUser, void (*callback)(void));
void UITAZoneModule_SetStartupModule(EntityUIControl *control, uint8 characterID, uint32 zoneID, uint8 actID, int32 score);
void UITAZoneModule_State_ExpandModule(void);
void UITAZoneModule_State_ContractModule(void);
void UITAZoneModule_State_MoveOffScreen(void);
void UITAZoneModule_State_Inactive(void);
void UITAZoneModule_State_ComeBackOnScreen(void);
#endif

#endif //! OBJ_UITAZONEMODULE_H
