#ifndef OBJ_ANNOUNCER_H
#define OBJ_ANNOUNCER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 spriteIndex;
    uint16 sfx_Three;
    uint16 sfx_Two;
    uint16 sfx_One;
    uint16 sfx_Go;
    uint16 sfx_Goal;
    uint16 sfx_SonicWins;
    uint16 sfx_TailsWins;
    uint16 sfx_KnuxWins;
#if RETRO_USE_PLUS
    uint16 sfx_MightyWins;
    uint16 sfx_RayWins;
#endif
    uint16 sfx_NewRecordTop;
    uint16 sfx_NewRecordMid;
    uint16 sfx_Sonic;
    uint16 sfx_Tails;
    uint16 sfx_Knuckles;
#if RETRO_USE_PLUS
    uint16 sfx_Mighty;
    uint16 sfx_Ray;
#endif
    uint16 sfx_TheWinnerIs;
    uint16 sfx_Player1;
    uint16 sfx_Player2;
#if RETRO_USE_PLUS
    uint16 sfx_Player3;
    uint16 sfx_Player4;
#endif
    uint16 sfx_ItsADraw;
    uint16 sfx_ItsADraw_Set;
    bool32 finishedCountdown;
} ObjectAnnouncer;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int32 timer;
    int32 playerID;
    int32 screen;
    Vector2 pos;
    Animator animator;
    Animator animator2;
} EntityAnnouncer;

// Object Struct
extern ObjectAnnouncer *Announcer;

// Standard Entity Events
void Announcer_Update(void);
void Announcer_LateUpdate(void);
void Announcer_StaticUpdate(void);
void Announcer_Draw(void);
void Announcer_Create(void* data);
void Announcer_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void Announcer_EditorDraw(void);
void Announcer_EditorLoad(void);
#endif
void Announcer_Serialize(void);

// Extra Entity Functions
void Announcer_StartCountdown(void);
void Announcer_AnnounceGoal(int32 screen);
void Announcer_Unknown3(void);
void Announcer_Unknown4(void);
void Announcer_Unknown5(void);
void Announcer_Unknown6(void);
void Announcer_State_AnnounceWinner(void);
void Announcer_State_AnnounceDraw(void);
void Announcer_State_AnnounceWinPlayer(void);

#endif //!OBJ_ANNOUNCER_H
