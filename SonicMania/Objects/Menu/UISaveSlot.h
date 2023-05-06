#ifndef OBJ_UISAVESLOT_H
#define OBJ_UISAVESLOT_H

#include "Game.h"

typedef enum {
    UISAVESLOT_REGULAR,
    UISAVESLOT_NOSAVE,
} UISaveSlotTypes;

// Object Class
struct ObjectUISaveSlot {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityUISaveSlot {
    MANIA_UI_ITEM_BASE
    bool32 isNewSave;
#if MANIA_USE_PLUS
    StateMachine(stateInput);
#endif
    int32 listID;
    int32 frameID;
    int32 saveZoneID;
    int32 saveLives;
#if MANIA_USE_PLUS
    int32 saveContinues;
#endif
    int32 saveEmeralds;
#if MANIA_USE_PLUS
    uint8 saveEncorePlayer;
    uint8 saveEncoreBuddy;
    uint8 saveEncoreFriends[3];
#endif
    UISaveSlotTypes type;
    int32 slotID;
#if MANIA_USE_PLUS
    bool32 encoreMode;
#endif
    bool32 currentlySelected;
    int32 zoneIconSprX;
    int32 textBounceOffset;
    int32 buttonBounceOffset;
    int32 textBouncePos;
    int32 buttonBouncePos;
    int32 fxRadius;
#if MANIA_USE_PLUS
    EntityFXRuby *fxRuby;
    bool32 debugEncoreDraw;
    uint8 dCharPoint;
    uint8 dCharPartner;
    uint8 dCharStock1;
    uint8 dCharStock2;
    uint8 dCharStock3;
#endif
    Animator uiAnimator;
    Animator playersAnimator;
    Animator shadowsAnimator;
    Animator livesAnimator;
#if MANIA_USE_PLUS
    Animator continuesAnimator;
#endif
    Animator emeraldsAnimator;
    Animator zoneIconAnimator;
    Animator zoneNameAnimator;
    Animator fuzzAnimator;
    Animator iconBGAnimator;
    Animator saveStatusAnimator;
    Animator numbersAnimator;
    uint16 textFrames;
};

// Object Struct
extern ObjectUISaveSlot *UISaveSlot;

// Standard Entity Events
void UISaveSlot_Update(void);
void UISaveSlot_LateUpdate(void);
void UISaveSlot_StaticUpdate(void);
void UISaveSlot_Draw(void);
void UISaveSlot_Create(void *data);
void UISaveSlot_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UISaveSlot_EditorDraw(void);
void UISaveSlot_EditorLoad(void);
#endif
void UISaveSlot_Serialize(void);

// Extra Entity Functions
#if MANIA_USE_PLUS
uint8 UISaveSlot_GetPlayerIDFromID(uint8 id);
uint8 UISaveSlot_GetIDFromPlayerID(uint8 playerID);
void UISaveSlot_DrawPlayerIcon_Encore(uint8 playerID, bool32 isSilhouette, uint8 buddyID, uint8 *friendIDs, uint8 friendCount, int32 drawX,
                                      int32 drawY);
#endif
void UISaveSlot_DrawPlayerIcons(int32 drawX, int32 drawY);
void UISaveSlot_DrawPlayerInfo(int32 drawX, int32 drawY);
void UISaveSlot_SetupButtonElements(void);
void UISaveSlot_SetupAnimators(void);
void UISaveSlot_LoadSaveInfo(void);
void UISaveSlot_HandleSaveIcons(void);

void UISaveSlot_DeleteDLG_CB(void);
#if MANIA_USE_PLUS
void UISaveSlot_DeleteSaveCB(bool32 success);
#else
void UISaveSlot_DeleteSaveCB(void);
#endif
void UISaveSlot_ProcessButtonCB(void);
void UISaveSlot_SelectedCB(void);

void UISaveSlot_NextCharacter(void);
void UISaveSlot_PrevCharacter(void);
void UISaveSlot_NextZone(void);
void UISaveSlot_PrevZone(void);

bool32 UISaveSlot_CheckButtonEnterCB(void);
bool32 UISaveSlot_CheckSelectedCB(void);
void UISaveSlot_ButtonEnterCB(void);
void UISaveSlot_ButtonLeaveCB(void);
void UISaveSlot_HandleSaveIconChange(void);

void UISaveSlot_State_NotSelected(void);
void UISaveSlot_State_OtherWasSelected(void);
void UISaveSlot_State_NewSave(void);
void UISaveSlot_State_ActiveSave(void);
#if MANIA_USE_PLUS
void UISaveSlot_StateInput_NewSave(void);
#endif
void UISaveSlot_State_CompletedSave(void);
void UISaveSlot_State_Selected(void);

#endif //! OBJ_UISAVESLOT_H
