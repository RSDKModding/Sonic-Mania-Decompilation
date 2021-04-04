#ifndef OBJ_SUMMARY_H
#define OBJ_SUMMARY_H

#include "../SonicMania.h"

#if RETRO_USE_PLUS
// Object Class
typedef struct {
    RSDK_OBJECT
    int timer; //= 240;
    int aniFrameID;
    int aniFrameDelay[4]; //= { 240, 3, 3, 3 };
} ObjectSummary;

// Entity Class
typedef struct {
    RSDK_ENTITY
    void(*state)(void);
    void(*stateDraw)(void);
    int timer;
    int field_64;
    int field_68;
    int player1ID;
    int player2ID;
    void *gameModeText;
    void *saveText;
    void *totalTime;
    void *textEntities[32];
    void *pictureA;
    void *pictureB;
    void *unknownPtr;
} EntitySummary;

// Object Struct
extern ObjectSummary *Summary;

// Standard Entity Events
void Summary_Update(void);
void Summary_LateUpdate(void);
void Summary_StaticUpdate(void);
void Summary_Draw(void);
void Summary_Create(void* data);
void Summary_StageLoad(void);
void Summary_EditorDraw(void);
void Summary_EditorLoad(void);
void Summary_Serialize(void);

// Extra Entity Functions
void Summary_State_Draw(void);

void Summary_State_SetupText(void);
void Summary_State_Unknown(void);
void Summary_State_Unknown2(void);
void Summary_State_Unknown3(void);

void Summary_SetTextString(byte anim, void *text, const char *str);
void Summary_SetStageTime(char *buffer, int time);
void Summary_LoadTimes(void);

#endif

#endif //!OBJ_SUMMARY_H
