#ifndef OBJ_HUD_H
#define OBJ_HUD_H

#include "../SonicMania.hpp"

// Object Class
struct ObjectHUD : Object {
    ushort hudMappings;
    ushort superButtonMappings;
    ushort sfx_Click;
    ushort sfx_Starpost;
    int dwordC;
    int gap10;
    int compSession[4];
    int field_24;
    int field_28;
    int field_2C;
    int field_30;
    int field_34;
};

// Entity Class
struct EntityHUD : Entity {
    void (*state)(void);
    Vector2 dword5C[4];
    int field_7C[4];
    int playerLives[4];
    int field_9C;
    int field_A0;
    void (*competitionStates[4])(void);
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
    EntityAnimationData hudElementsData;
    EntityAnimationData numbersData;
    EntityAnimationData hyperNumbersData;
    EntityAnimationData lifeIconsData;
    EntityAnimationData playerIDData;
    EntityAnimationData superButtonsData;
    EntityAnimationData superButtonData1;
    EntityAnimationData taData1;
    EntityAnimationData superButtonData2;
    EntityAnimationData taData2;
    EntityAnimationData superButtonData3;
};

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
void HUD_GetKeyFrame(EntityAnimationData *data, int buttonID);
void HUD_GetSuperFrames();
void HUD_Unknown5();
void HUD_Unknown6();

#endif //!OBJ_HUD_H
