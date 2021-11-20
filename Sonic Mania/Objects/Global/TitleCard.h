#ifndef OBJ_TITLECARD_H
#define OBJ_TITLECARD_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    StateMachine(finishedCB);
    StateMachine(suppressCB);
} ObjectTitleCard;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int32 actionTimer;
    int32 timer;
    Vector2 decorationPos;
    int32 barPos[4];
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
    int32 zoneCharPos[4];
    int32 zoneCharSpeed[4];
    int32 zoneStopPos;
    Vector2 charPos[20];
    int32 charSpeeds[20];
    int32 word2Offset;
    int32 topWordOffset;
    int32 bottomWordOffset;
    int32 currentWordPos;
    int32 wordStopPos;
    uint8 actID;
    int32 actNumScale;
    Vector2 drawPos2;
    bool32 enableIntro;
    Animator decorationData;
    Animator nameLetterData;
    Animator zoneLetterData;
    Animator actNumbersData;
    colour colours[5];
} EntityTitleCard;

// Object Struct
extern ObjectTitleCard *TitleCard;

// Standard Entity Events
void TitleCard_Update(void);
void TitleCard_LateUpdate(void);
void TitleCard_StaticUpdate(void);
void TitleCard_Draw(void);
void TitleCard_Create(void *data);
void TitleCard_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void TitleCard_EditorDraw(void);
void TitleCard_EditorLoad(void);
#endif
void TitleCard_Serialize(void);

// Extra Entity Functions
void TitleCard_SetColours(void);
void TitleCard_SetPoints(void);
void TitleCard_SetWordPositions(void);
void TitleCard_CheckPointBoundaries(void);
void TitleCard_HandleZoneCharacters(void);
void TitleCard_SetCamera(void);

// States
void TitleCard_State_Initial(void);
void TitleCard_State_OpeningBG(void);
void TitleCard_State_ShowTitle(void);
void TitleCard_State_Idle(void);
void TitleCard_State_SlideAway(void);
void TitleCard_State_Supressed(void);
// Draw States
void TitleCard_Draw_Default(void);
void TitleCard_Draw_SolidBG(void);
void TitleCard_Draw_SlideAway(void);

#endif //! OBJ_TITLECARD_H
