#ifndef OBJ_TITLECARD_H
#define OBJ_TITLECARD_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort spriteIndex;
    int (*funcA)();
    void (*suppressCallback)();
} ObjectTitleCard;

// Entity Class
typedef struct {
    RSDK_ENTITY
    void (*state)();
    void (*stateDraw)();
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
    Vector2 field_1C8[4];
    int dword1E8;
    Vector2 charPos[20];
    int field_28C[20];
    int word2Offset;
    int field_2E0;
    int field_2E4;
    int field_2E8;
    int dword2EC;
    byte actID;
    int dword2F4;
    Vector2 drawPos2;
    bool32 enableIntro;
    AnimationData decorationData;
    AnimationData nameLetterData;
    AnimationData zoneLetterData;
    AnimationData actNumbersData;
    colour colours[5];
} EntityTitleCard;

// Object Struct
extern ObjectTitleCard *TitleCard;

// Standard Entity Events
void TitleCard_Update();
void TitleCard_LateUpdate();
void TitleCard_StaticUpdate();
void TitleCard_Draw();
void TitleCard_Create(void *data);
void TitleCard_StageLoad();
void TitleCard_EditorDraw();
void TitleCard_EditorLoad();
void TitleCard_Serialize();

// Extra Entity Functions
void TitleCard_SetColours();
void TitleCard_SetPoints();
void TitleCard_Unknown2();
void TitleCard_Unknown3();
void TitleCard_Unknown4();
void TitleCard_Unknown5();
void TitleCard_Unknown6(EntityPlayer *player);

// States
void TitleCard_Unknown7(EntityPlayer *player);
void TitleCard_Unknown8(EntityPlayer *player);
void TitleCard_Unknown9(EntityPlayer *player);
void TitleCard_Unknown10(EntityPlayer *player);
void TitleCard_Unknown11();
// Draw States
void TitleCard_Unknown12();
void TitleCard_Unknown13();
void TitleCard_Unknown14();

#endif //! OBJ_TITLECARD_H
