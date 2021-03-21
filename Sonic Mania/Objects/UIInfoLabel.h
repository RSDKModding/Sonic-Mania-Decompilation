#ifndef OBJ_UIINFOLABEL_H
#define OBJ_UIINFOLABEL_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort spriteIndex;
} ObjectUIInfoLabel;

// Entity Class
typedef struct {
    RSDK_ENTITY
    AnimationData data;
    int field_70;
    int field_74;
    Vector2 size;
    TextInfo text;
    int width;
    AnimationData data2;
} EntityUIInfoLabel;

// Object Struct
extern ObjectUIInfoLabel *UIInfoLabel;

// Standard Entity Events
void UIInfoLabel_Update();
void UIInfoLabel_LateUpdate();
void UIInfoLabel_StaticUpdate();
void UIInfoLabel_Draw();
void UIInfoLabel_Create(void* data);
void UIInfoLabel_StageLoad();
void UIInfoLabel_EditorDraw();
void UIInfoLabel_EditorLoad();
void UIInfoLabel_Serialize();

// Extra Entity Functions
void UIInfoLabel_Unknown1(EntityUIInfoLabel *label, char *text);
void UIInfoLabel_Unknown2(EntityUIInfoLabel *entity, TextInfo *text);
void UIInfoLabel_DrawSprites();

#endif //!OBJ_UIINFOLABEL_H
