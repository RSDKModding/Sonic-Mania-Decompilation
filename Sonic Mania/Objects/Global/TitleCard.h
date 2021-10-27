#ifndef OBJ_TITLECARD_H
#define OBJ_TITLECARD_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 spriteIndex;
    StateMachine(finishedCB);
    StateMachine(suppressCB);
} ObjectTitleCard;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(stateDraw);
    int32 field_60;
    int32 timer;
    Vector2 decorationPos;
    int32 dword70;
    int32 dword74;
    int32 dword78;
    int32 dword7C;
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
    int32 dword1E8;
    Vector2 charPos[20];
    int32 charSpeeds[20];
    int32 word2Offset;
    int32 field_2E0;
    int32 field_2E4;
    int32 field_2E8;
    int32 dword2EC;
    uint8 actID;
    int32 dword2F4;
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
void TitleCard_Unknown2(void);
void TitleCard_Unknown3(void);
void TitleCard_Unknown4(void);
void TitleCard_Unknown5(void);
void TitleCard_Unknown6(void);

// States
void TitleCard_Unknown7(void);
void TitleCard_Unknown8(void);
void TitleCard_Unknown9(void);
void TitleCard_Unknown10(void);
void TitleCard_Unknown11(void);
// Draw States
void TitleCard_StateDraw_Default(void);
void TitleCard_Unknown13(void);
void TitleCard_Unknown14(void);

#endif //! OBJ_TITLECARD_H
