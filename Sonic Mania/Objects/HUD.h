#ifndef OBJ_HUD_H
#define OBJ_HUD_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort hudMappings;
    ushort superButtonMappings;
    ushort sfx_Click;
    ushort sfx_Starpost;
    int dwordC;
    int gap10;
    int compSession[4];
    int field_24;
    int field_28[PLAYER_MAX];
} ObjectHUD;

// Entity Class
typedef struct {
    RSDK_ENTITY
    void (*state)(void);
    Vector2 dword5C[4];
    int field_7C[4];
    int playerLives[PLAYER_MAX];
    int field_9C;
    int field_A0;
    void (*competitionStates[PLAYER_MAX])();
    Vector2 dwordB4[4];
    Vector2 dwordD4[4];
    Vector2 dwordF4[4];
    Vector2 dword114[4];
    int field_134[4];
    int screenID;
    int timeFlashFrame;
    int ringFlashFrame;
    int field_150;
    int field_154;
    AnimationData hudElementsData;
    AnimationData numbersData;
    AnimationData hyperNumbersData;
    AnimationData lifeIconsData;
    AnimationData playerIDData;
    AnimationData superButtonsData;
    AnimationData superButtonData1;
    AnimationData taData1;
    AnimationData superButtonData2;
    AnimationData taData2;
    AnimationData superButtonData3;
} EntityHUD;

// Object Struct
extern ObjectHUD *HUD;

// Standard Entity Events
void HUD_Update();
void HUD_LateUpdate();
void HUD_StaticUpdate();
void HUD_Draw();
void HUD_Create(void* data);
void HUD_StageLoad();
void HUD_EditorDraw();
void HUD_EditorLoad();
void HUD_Serialize();

// Extra Entity Functions
void HUD_DrawNumbersBase10(Vector2 *drawPos, int value, signed int maxDigits);
void HUD_DrawNumbersBase16(Vector2 *drawPos, int value);
void HUD_DrawNumbersHyperRing(Vector2 *drawPos, int value);
void HUD_GetKeyFrame(AnimationData *data, int buttonID);
void HUD_GetSuperFrames();
void HUD_Unknown5();
void HUD_Unknown6();

#endif //!OBJ_HUD_H
