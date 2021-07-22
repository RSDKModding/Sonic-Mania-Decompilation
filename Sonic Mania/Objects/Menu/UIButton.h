#ifndef OBJ_UIBUTTON_H
#define OBJ_UIBUTTON_H

#include "SonicMania.h"


// Object Class
typedef struct {
	RSDK_OBJECT
} ObjectUIButton;

// Entity Class
typedef struct {
    MANIA_UI_ITEM_BASE
    int align;
    int choiceCount;
    byte choiceDir;
    bool32 invisible;
    bool32 assignsP1;
    bool32 freeBindP2;
    bool32 transition;
    bool32 stopMusic;
    bool32 isDisabled;
    int dword138;
    int field_13C;
    int field_140;
    int field_144;
    int field_148;
    int dword14C;
    int field_150;
    Vector2 posUnknown;
    int field_15C;
    int field_160;
    Animator animator;
    ushort textSpriteIndex;
    int startListID;
    int startFrameID;
} EntityUIButton;

// Object Struct
extern ObjectUIButton *UIButton;

// Standard Entity Events
void UIButton_Update(void);
void UIButton_LateUpdate(void);
void UIButton_StaticUpdate(void);
void UIButton_Draw(void);
void UIButton_Create(void* data);
void UIButton_StageLoad(void);
void UIButton_EditorDraw(void);
void UIButton_EditorLoad(void);
void UIButton_Serialize(void);

// Extra Entity Functions
void UIButton_Unknown1(EntityUIButton *button);
Entity *UIButton_Unknown2(EntityUIButton *button, int a2);
void UIButton_Fail(void);

#endif //!OBJ_UIBUTTON_H
