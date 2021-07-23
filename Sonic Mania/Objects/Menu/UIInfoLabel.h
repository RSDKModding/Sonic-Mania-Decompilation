#ifndef OBJ_UIINFOLABEL_H
#define OBJ_UIINFOLABEL_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort spriteIndex;
} ObjectUIInfoLabel;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Animator animator;
    int field_70;
    int field_74;
    Vector2 size;
    TextInfo text;
    int width;
    Animator data2;
} EntityUIInfoLabel;

// Object Struct
extern ObjectUIInfoLabel *UIInfoLabel;

// Standard Entity Events
void UIInfoLabel_Update(void);
void UIInfoLabel_LateUpdate(void);
void UIInfoLabel_StaticUpdate(void);
void UIInfoLabel_Draw(void);
void UIInfoLabel_Create(void* data);
void UIInfoLabel_StageLoad(void);
void UIInfoLabel_EditorDraw(void);
void UIInfoLabel_EditorLoad(void);
void UIInfoLabel_Serialize(void);

// Extra Entity Functions
void UIInfoLabel_SetText(EntityUIInfoLabel *label, char *text);
void UIInfoLabel_SetString(EntityUIInfoLabel *entity, TextInfo *text);
void UIInfoLabel_DrawSprites(void);

#endif //!OBJ_UIINFOLABEL_H
