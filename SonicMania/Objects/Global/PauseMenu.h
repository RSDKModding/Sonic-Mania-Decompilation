#ifndef OBJ_PAUSEMENU_H
#define OBJ_PAUSEMENU_H

#include "Game.h"

#define PAUSEMENU_BUTTON_COUNT (3)

// Object Class
struct ObjectPauseMenu {
    RSDK_OBJECT
    uint16 sfxBleep;
    uint16 sfxAccept;
    bool32 disableEvents;
    bool32 controllerDisconnect;
    bool32 forcedDisconnect;
    bool32 signOutDetected;
#if MANIA_USE_PLUS
    bool32 plusChanged;
#endif
    bool32 activeChannels[0x10];
#if MANIA_USE_PLUS
    uint16 tintLookupTable[0x10000];
#endif
};

// Entity Class
struct EntityPauseMenu {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int32 timer;
    int32 tintAlpha;
    Vector2 headerPos;
    Vector2 yellowTrianglePos;
    EntityUIControl *manager;
    uint8 triggerPlayer;
    bool32 disableRestart;
    int32 buttonCount;
    uint8 buttonIDs[PAUSEMENU_BUTTON_COUNT];
    void (*buttonActions[PAUSEMENU_BUTTON_COUNT])(void);
    EntityUIButton *buttonPtrs[PAUSEMENU_BUTTON_COUNT];
    int32 paused;
    int32 fadeTimer;
    bool32 (*disconnectCheck)(void);
    int32 forcePaused;
    Animator animator;
    void (*fadeoutCB)(void);
    int32 unused1;
    int32 unused2; // these may possibly be leftover or editor things
};

// Object Struct
extern ObjectPauseMenu *PauseMenu;

// Standard Entity Events
void PauseMenu_Update(void);
void PauseMenu_LateUpdate(void);
void PauseMenu_StaticUpdate(void);
void PauseMenu_Draw(void);
void PauseMenu_Create(void *data);
void PauseMenu_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PauseMenu_EditorDraw(void);
void PauseMenu_EditorLoad(void);
#endif
void PauseMenu_Serialize(void);

// Helper Functions
void PauseMenu_SetupMenu(void);
void PauseMenu_SetupTintTable(void);

void PauseMenu_AddButton(uint8 id, void *action);
void PauseMenu_ClearButtons(EntityPauseMenu *entity);
void PauseMenu_HandleButtonPositions(void);

void PauseMenu_PauseSound(void);
void PauseMenu_ResumeSound(void);
void PauseMenu_StopSound(void);

void PauseMenu_FocusCamera(void);
void PauseMenu_UpdateCameras(void);

void PauseMenu_CheckAndReassignControllers(void);
bool32 PauseMenu_IsDisconnected(void);

uint8 PauseMenu_GetPlayerCount(void);

// Callbacks
void PauseMenu_ResumeButtonCB(void);
void PauseMenu_RestartButtonCB(void);
void PauseMenu_ExitButtonCB(void);

void PauseMenu_RestartDialog_YesCB(void);
void PauseMenu_ExitDialog_YesCB(void);
void PauseMenu_RestartFadeCB(void);
void PauseMenu_ExitFadeCB(void);

void PauseMenu_ActionCB_Button(void);

// States
void PauseMenu_State_SetupButtons(void);

void PauseMenu_State_StartPause(void);
void PauseMenu_State_StartPauseCompetition(void);

void PauseMenu_State_Paused(void);
void PauseMenu_State_ForcedPause(void);
void PauseMenu_State_ForcedPauseCompetition(void);

void PauseMenu_State_Resume(void);
void PauseMenu_State_ResumeCompetition(void);
void PauseMenu_State_ForcedResumeCompetition(void);

void PauseMenu_State_SetupTitleFade(void);
void PauseMenu_State_FadeToTitle(void);
void PauseMenu_State_HandleFadeout(void);

// Draw States
void PauseMenu_DrawPauseMenu(void);
void PauseMenu_Draw_RegularPause(void);

void PauseMenu_Draw_ForcePause(void);

#endif //! OBJ_PAUSEMENU_H
