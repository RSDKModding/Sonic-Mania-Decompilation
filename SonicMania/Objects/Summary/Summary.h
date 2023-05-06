#ifndef OBJ_SUMMARY_H
#define OBJ_SUMMARY_H

#include "Game.h"

#if MANIA_USE_PLUS
// Object Class
struct ObjectSummary {
    RSDK_OBJECT
    STATIC(int32 bgAniDuration, 240);
    int32 bgAniFrame;
    TABLE(int32 bgAniDurationTable[4], { 240, 3, 3, 3 });
};

// Entity Class
struct EntitySummary {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int32 timer;
    int32 unused1;
    int32 unused2;
    int32 leaderCharacterID;
    int32 sidekickCharacterID;
    EntityUIText *gameModeLabel;
    EntityUIText *saveFileLabel;
    EntityUIText *totalTime;
    EntityUIText *zoneLabels[32];
    EntityUIPicture *player2Icon;
    EntityUIPicture *player1Icon;
    void *unusedPtr;
};

// Object Struct
extern ObjectSummary *Summary;

// Standard Entity Events
void Summary_Update(void);
void Summary_LateUpdate(void);
void Summary_StaticUpdate(void);
void Summary_Draw(void);
void Summary_Create(void *data);
void Summary_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Summary_EditorDraw(void);
void Summary_EditorLoad(void);
#endif
void Summary_Serialize(void);

// Extra Entity Functions
void Summary_State_Draw(void);

void Summary_State_SetupText(void);
void Summary_State_FadeIn(void);
void Summary_State_Wait(void);
void Summary_State_FadeOut(void);

void Summary_SetTextString(uint8 anim, EntityUIText *text, const char *str);
void Summary_GetPlayTime(char *buffer, int32 time);
void Summary_LoadTimes(void);

#endif

#endif //! OBJ_SUMMARY_H
