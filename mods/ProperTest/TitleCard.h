#pragma once
#ifndef OBJ_TITLECARD_H
#define OBJ_TITLECARD_H

#include "ProperTest.h"

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int field_60;
    int timer;
    Vector2 decorationPos;
    int dword70;
    int dword74;
    int dword78;
    int dword7C;
    Vector2 points0[4];
    Vector2 points1[4];
    Vector2 points2[4];
    Vector2 points3[4];
    Vector2 points4[4];
    Vector2 points5[4];
    Vector2 points6[4];
    Vector2 points7[4];
    Vector2 points8[4];
    Vector2 points9[4];
    TextInfo zoneName;
    int zoneCharPos[4];
    int zoneCharSpeed[4];
    int dword1E8;
    Vector2 charPos[20];
    int charSpeeds[20];
    int word2Offset;
    int field_2E0;
    int field_2E4;
    int field_2E8;
    int dword2EC;
    byte actID;
    int dword2F4;
    Vector2 drawPos2;
    bool32 enableIntro;
    Animator decorationData;
    Animator nameLetterData;
    Animator zoneLetterData;
    Animator actNumbersData;
    colour colours[5];
} EntityTitleCard;

// Standard Entity Events
void TitleCard_Create(void* data);
void TitleCard_Draw(void);

void SetStageColors(const char *stage, int *c, int count, int filter);
void SetBaseColors(int *c, int count, int filter);
void ResetColors();
void LoadFromSettings(const char *id);

#endif //! OBJ_TITLECARD_H
