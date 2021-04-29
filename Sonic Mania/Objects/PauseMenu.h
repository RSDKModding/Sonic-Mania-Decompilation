#ifndef OBJ_PAUSEMENU_H
#define OBJ_PAUSEMENU_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort sfxBleep;
    ushort sfxAccept;
    int disableEvents;
    bool32 controllerDisconnect;
    int dword10;
    bool32 signoutDetected;
    bool32 plusChanged;
    bool32 channelFlags[0x10];
    ushort lookupTable[0x10000];
} ObjectPauseMenu;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int timer;
    int field_64;
    Vector2 field_68;
    Vector2 field_70;
    Entity *manager;
    byte triggerPlayer;
    bool32 disableRestart;
    int buttonCount;
    byte buttonIDs[3];
    void *buttonActions[3];
    Entity* buttonPtrs[3];
    int field_A4;
    int fillTimer;
    bool32 (*field_AC)(void);
    int field_B0;
    Animator animator;
    void (*funcPtrUnknown)(void);
    int field_D0;
    int field_D4;
} EntityPauseMenu;

// Object Struct
extern ObjectPauseMenu *PauseMenu;

// Standard Entity Events
void PauseMenu_Update(void);
void PauseMenu_LateUpdate(void);
void PauseMenu_StaticUpdate(void);
void PauseMenu_Draw(void);
void PauseMenu_Create(void* data);
void PauseMenu_StageLoad(void);
void PauseMenu_EditorDraw(void);
void PauseMenu_EditorLoad(void);
void PauseMenu_Serialize(void);

// Extra Entity Functions
byte PauseMenu_GetPlayerCount(void);
void PauseMenu_SetupLookupTable(void);
void PauseMenu_Unknown3(void);
void PauseMenu_Unknown4(void);
void PauseMenu_AddButton(byte id, void *action);
void PauseMenu_Unknown6(void);
void PauseMenu_ClearButtons(EntityPauseMenu *entity);
void PauseMenu_Unknown8(void);
void PauseMenu_Unknown9(void);
void PauseMenu_FocusCamera(void);
void PauseMenu_UpdateCameras(void);
void PauseMenu_Resume_CB(void);
void PauseMenu_Restart_CB(void);
void PauseMenu_Unknown13(void);
void PauseMenu_Exit_CB(void);
void PauseMenu_Unknown15(void);
void PauseMenu_Unknown16(void);
void PauseMenu_PauseSound(void);
void PauseMenu_ResumeSound(void);
void PauseMenu_StopSound(void);
void PauseMenu_SetupButtons(void);
void PauseMenu_Unknown21(void);
void PauseMenu_Unknown22(void);
void PauseMenu_Unknown23(void);
void PauseMenu_Unknown24(void);
void PauseMenu_Unknown26(void);
void PauseMenu_Unknown27(void);
void PauseMenu_Unknown28(void);
void PauseMenu_Unknown29(void);
void PauseMenu_Unknown31(void);
bool32 PauseMenu_Unknown32(void);
void PauseMenu_Unknown33(void);
void PauseMenu_Unknown34(void);
void PauseMenu_Unknown35(void);
void PauseMenu_Unknown36(void);
void PauseMenu_Unknown37(void);

#endif //!OBJ_PAUSEMENU_H
