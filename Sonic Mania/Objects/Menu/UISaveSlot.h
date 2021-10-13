#ifndef OBJ_UISAVESLOT_H
#define OBJ_UISAVESLOT_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectUISaveSlot;

// Entity Class
typedef struct {
    MANIA_UI_ITEM_BASE
    bool32 isNewSave;
#if RETRO_USE_PLUS
    StateMachine(stateUnknown);
#endif
    int32 listID;
    int32 frameID;
    int32 saveZoneID;
    int32 saveLives;
#if RETRO_USE_PLUS
    int32 saveContinues;
#endif
    int32 saveEmeralds;
#if RETRO_USE_PLUS
    uint8 saveEncorePlayer;
    uint8 saveEncoreBuddy;
    uint8 saveEncoreFriends[3];
#endif
    int32 type;
    int32 slotID;
#if RETRO_USE_PLUS
    bool32 encoreMode;
#endif
    int32 zoneID;
    int32 field_13C;
    int32 field_140;
    int32 field_144;
    int32 field_148;
    int32 field_14C;
    int32 radius;
    #if RETRO_USE_PLUS
    EntityFXRuby *fxRuby;
    bool32 debugEncoreDraw;
    uint8 dCharPoint;
    uint8 dCharPartner;
    uint8 dCharStock1;
    uint8 dCharStock2;
    uint8 dCharStock3;
    #endif
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
    uint16 textSpriteIndex;
} EntityUISaveSlot;

// Object Struct
extern ObjectUISaveSlot *UISaveSlot;

// Standard Entity Events
void UISaveSlot_Update(void);
void UISaveSlot_LateUpdate(void);
void UISaveSlot_StaticUpdate(void);
void UISaveSlot_Draw(void);
void UISaveSlot_Create(void* data);
void UISaveSlot_StageLoad(void);
void UISaveSlot_EditorDraw(void);
void UISaveSlot_EditorLoad(void);
void UISaveSlot_Serialize(void);

// Extra Entity Functions
#if RETRO_USE_PLUS
uint8 UISaveSlot_Unknown1(uint8 id);
void UISaveSlot_Unknown2(uint8 playerID, bool32 isSilhouette, uint8 buddyID, uint8 *friendIDs, uint8 friendCount, int32 drawX, int32 drawY);
#endif
void UISaveSlot_Unknown3(int32 drawX, int32 drawY);
void UISaveSlot_DrawPlayerInfo(int32 drawX, int32 drawY);
void UISaveSlot_SetupButtonElements(void);
void UISaveSlot_Unknown6(void);
void UISaveSlot_LoadSaveInfo(void);
void UISaveSlot_Unknown8(void);
void UISaveSlot_DeleteDLG_CB(void);
void UISaveSlot_DeleteSaveCB(int32 status);
void UISaveSlot_ProcessButtonCB(void);
void UISaveSlot_SelectedSave(void);
void UISaveSlot_NextCharacter(void);
void UISaveSlot_PrevCharacter(void);
void UISaveSlot_NextZone(void);
void UISaveSlot_PrevZone(void);
bool32 UISaveSlot_Unknown17(void);
bool32 UISaveSlot_Unknown18(void);
void UISaveSlot_Unknown19(void);
void UISaveSlot_Unknown20(void);
void UISaveSlot_Unknown21(void);
void UISaveSlot_Unknown22(void);
void UISaveSlot_Unknown23(void);
void UISaveSlot_Unknown24(void);
void UISaveSlot_Unknown25(void);
void UISaveSlot_Unknown26(void);
void UISaveSlot_Unknown27(void);
void UISaveSlot_Unknown28(void);


#endif //!OBJ_UISAVESLOT_H
