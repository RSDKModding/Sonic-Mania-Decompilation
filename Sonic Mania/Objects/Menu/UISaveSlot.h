#ifndef OBJ_UISAVESLOT_H
#define OBJ_UISAVESLOT_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort aniFrames;
} ObjectUISaveSlot;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    void (*processButtonCB)(void);
    bool32 (*touchCB)(void);
    void (*options2)(void);
    void (*options3)(void);
    void (*failCB)(void);
    void (*options5)(void);
    void (*options6)(void);
    bool32 (*options7)(void);
    bool32 (*options8)(void);
    int timer;
    Vector2 posUnknown2;
    Entity *parent;
    Vector2 touchPosStart;
    Vector2 touchPosEnd;
    bool32 touchPressed;
    Vector2 touchPos1[4];
    Vector2 touchPos2[4];
    void (*touchPosCallbacks[4])(void);
    int touchPosCount;
    int touchCountUnknown;  
    bool32 flag;
    bool32 disabled;
    // Split
    bool32 isNewSave;
#if RETRO_USE_PLUS
    StateMachine(stateUnknown);
#endif
    int listID;
    int frameID;
    int saveZoneID;
    int saveLives;
#if RETRO_USE_PLUS
    int saveContinues;
#endif
    int saveEmeralds;
#if RETRO_USE_PLUS
    byte saveEncorePlayer;
    byte saveEncoreBuddy;
    byte saveEncoreFriends[3];
#endif
    int type;
    int slotID;
#if RETRO_USE_PLUS
    bool32 encoreMode;
#endif
    int zoneID;
    int field_13C;
    int field_140;
    int field_144;
    int field_148;
    int field_14C;
    int radius;
    #if RETRO_USE_PLUS
    EntityFXRuby *fxRuby;
    bool32 debugEncoreDraw;
    byte dCharPoint;
    byte dCharPartner;
    byte dCharStock1;
    byte dCharStock2;
    byte dCharStock3;
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
    ushort textSpriteIndex;
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
byte UISaveSlot_Unknown1(byte id);
#if RETRO_USE_PLUS
void UISaveSlot_Unknown2(byte playerID, bool32 isSilhouette, byte buddyID, byte *friendIDs, byte friendCount, int drawX, int drawY);
#endif
void UISaveSlot_Unknown3(int drawX, int drawY);
void UISaveSlot_DrawPlayerInfo(int drawX, int drawY);
void UISaveSlot_SetupButtonElements(void);
void UISaveSlot_Unknown6(void);
void UISaveSlot_LoadSaveInfo(void);
void UISaveSlot_Unknown8(void);
void UISaveSlot_DeleteDLG_CB(void);
void UISaveSlot_DeleteSaveCB(int status);
void UISaveSlot_Unknown9(void);
void UISaveSlot_Unknown12(void);
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
