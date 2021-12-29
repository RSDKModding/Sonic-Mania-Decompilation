#ifndef OBJ_SUMMARY_H
#define OBJ_SUMMARY_H

#include "SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
struct ObjectSummary {
    RSDK_OBJECT
    STATIC(int32 timer, 240);
    int32 aniFrameID;
    TABLE(int32 aniFrameDelay[4], { 240, 3, 3, 3 });
};

// Entity Class
struct EntitySummary {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int32 timer;
    int32 unused1;
    int32 unused2;
    int32 player1ID;
    int32 player2ID;
    void *gameModeText;
    void *saveText;
    void *totalTime;
    void *textEntities[32];
    void *pictureA;
    void *pictureB;
    void *unusedPtr;
};

// Object Struct
extern ObjectSummary *Summary;

// Standard Entity Events
void Summary_Update(void);
void Summary_LateUpdate(void);
void Summary_StaticUpdate(void);
void Summary_Draw(void);
void Summary_Create(void* data);
void Summary_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
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

void Summary_SetTextString(uint8 anim, void *text, const char *str);
void Summary_SetStageTime(char *buffer, int32 time);
void Summary_LoadTimes(void);

#endif

#endif //!OBJ_SUMMARY_H
