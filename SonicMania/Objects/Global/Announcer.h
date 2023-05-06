#ifndef OBJ_ANNOUNCER_H
#define OBJ_ANNOUNCER_H

#include "Game.h"

// Object Class
struct ObjectAnnouncer {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxThree;
    uint16 sfxTwo;
    uint16 sfxOne;
    uint16 sfxGo;
    uint16 sfxGoal;
    uint16 sfxSonicWins;
    uint16 sfxTailsWins;
    uint16 sfxKnuxWins;
#if MANIA_USE_PLUS
    uint16 sfxMightyWins;
    uint16 sfxRayWins;
#endif
    uint16 sfxNewRecordTop;
    uint16 sfxNewRecordMid;
    uint16 sfxSonic;
    uint16 sfxTails;
    uint16 sfxKnuckles;
#if MANIA_USE_PLUS
    uint16 sfxMighty;
    uint16 sfxRay;
#endif
    uint16 sfxTheWinnerIs;
    uint16 sfxPlayer1;
    uint16 sfxPlayer2;
#if MANIA_USE_PLUS
    uint16 sfxPlayer3;
    uint16 sfxPlayer4;
#endif
    uint16 sfxDrawRound;
    uint16 sfxDrawSet;
    bool32 finishedCountdown;
};

// Entity Class
struct EntityAnnouncer {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int32 timer;
    int32 playerID;
    int32 screen;
    Vector2 drawOffset;
    Animator animator;
    Animator playerIconAnimator;
};

// Object Struct
extern ObjectAnnouncer *Announcer;

// Standard Entity Events
void Announcer_Update(void);
void Announcer_LateUpdate(void);
void Announcer_StaticUpdate(void);
void Announcer_Draw(void);
void Announcer_Create(void *data);
void Announcer_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Announcer_EditorDraw(void);
void Announcer_EditorLoad(void);
#endif
void Announcer_Serialize(void);

// Extra Entity Functions
void Announcer_StartCountdown(void);
void Announcer_AnnounceGoal(int32 screen);

void Announcer_Draw_Countdown(void);
void Announcer_Draw_Finished(void);

void Announcer_State_Countdown(void);
void Announcer_State_Finished(void);
void Announcer_State_AnnounceWinner(void);
void Announcer_State_AnnounceDraw(void);
void Announcer_State_AnnounceWinPlayer(void);

#endif //! OBJ_ANNOUNCER_H
