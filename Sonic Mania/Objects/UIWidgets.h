#ifndef OBJ_UIWIDGETS_H
#define OBJ_UIWIDGETS_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int array1[0x10];
    int arrayIndex;
    int field_48[12];
    AnimationData animData1;
    AnimationData animData2;
    ushort uiSpriteIndex;
    ushort saveSelectSpriteIndex;
    ushort textSpriteIndex;
    ushort labelSpriteIndex;
    ushort sfx_Bleep;
    ushort sfx_Accept;
    ushort sfx_Warp;
    ushort sfx_Event;
    ushort sfx_Woosh;
    ushort sfx_Fail;
    int value;
} ObjectUIWidgets;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int field_58;
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
void UIWidgets_EditorDraw(void);
void UIWidgets_EditorLoad(void);
void UIWidgets_Serialize(void);

// Extra Entity Functions
void UIWidgets_ApplyLanguage(void);

void UIWidgets_Unknown2(int a1, int a2, int a3, int a4);
void UIWidgets_Unknown3(int a1, int a2, int a3, int a4);
void UIWidgets_Unknown4(int a1, int a2, int a3, int a4);
void UIWidgets_Unknown5(int red, int a2, int green, int blue, int x, int y);
void UIWidgets_Unknown6(char a1, int red, int green, int blue, InkEffects ink, int x, int y);
void UIWidgets_Unknown7(int a1, int a2, int a3, int red, int green, int blue, int a7, int a8);
void UIWidgets_Unknown8(int x, int y);
void UIWidgets_Unknown9(int x, int y);
int UIWidgets_Unknown10(int a1, int a2, int a3, int a4);
void UIWidgets_Unknown11(int minutes, int seconds, int milliseconds, int x, int y);

#endif //!OBJ_UIWIDGETS_H
