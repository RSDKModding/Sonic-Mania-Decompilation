#ifndef OBJ_UIWIDGETS_H
#define OBJ_UIWIDGETS_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
#if RETRO_USE_PLUS
    int32 buttonColours[16];
    int32 arrayIndex;
    int32 field_48[12];
#else
    int32 arrayIndex;
    int32 buttonColours[16];
#endif
    Animator animator1;
    Animator animator2;
    uint16 uiSpriteIndex;
#if RETRO_USE_PLUS
    uint16 saveSelectSpriteIndex;
#endif
    uint16 textSpriteIndex;
    uint16 labelSpriteIndex;
    uint16 sfxBleep;
    uint16 sfxAccept;
    uint16 sfxWarp;
    uint16 sfxEvent;
    uint16 sfxWoosh;
    uint16 sfxFail;
#if RETRO_USE_PLUS
    colour buttonColour;
#endif
} ObjectUIWidgets;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int32 field_58;
} EntityUIWidgets;

// Object Struct
extern ObjectUIWidgets *UIWidgets;

// Standard Entity Events
void UIWidgets_Update(void);
void UIWidgets_LateUpdate(void);
void UIWidgets_StaticUpdate(void);
void UIWidgets_Draw(void);
void UIWidgets_Create(void* data);
void UIWidgets_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void UIWidgets_EditorDraw(void);
void UIWidgets_EditorLoad(void);
#endif
void UIWidgets_Serialize(void);

// Extra Entity Functions
void UIWidgets_ApplyLanguage(void);

void UIWidgets_Unknown2(int32 a1, int32 a2, int32 a3, int32 a4);
void UIWidgets_Unknown3(int32 a1, int32 a2, int32 a3, int32 a4);
void UIWidgets_Unknown4(int32 a1, int32 a2, int32 a3, int32 a4);
void UIWidgets_DrawRightTriangle(int32 x, int32 y, int32 a2, int32 red, int32 green, int32 blue);
void UIWidgets_Unknown6(char a1, int32 red, int32 green, int32 blue, InkEffects ink, int32 x, int32 y);
void UIWidgets_Unknown7(int32 a1, int32 a2, int32 a3, int32 red, int32 green, int32 blue, int32 a7, int32 a8);
void UIWidgets_Unknown8(int32 a1, int32 x, int32 y);
void UIWidgets_Unknown9(int32 a1, int32 x, int32 y);
Vector2 UIWidgets_Unknown10(colour colour1, colour colour2, int32 drawX, int32 drawY);
#if RETRO_USE_PLUS
void UIWidgets_Unknown11(int32 minutes, int32 seconds, int32 milliseconds, int32 x, int32 y);
#endif

#endif //!OBJ_UIWIDGETS_H
