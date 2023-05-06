#ifndef OBJ_UIINFOLABEL_H
#define OBJ_UIINFOLABEL_H

#include "Game.h"

// Object Class
struct ObjectUIInfoLabel {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityUIInfoLabel {
    RSDK_ENTITY
    Animator unusedAnimator;
    int32 unused1;
    int32 unused2;
    Vector2 size;
    String text;
    int32 width;
    Animator textAnimator;
};

// Object Struct
extern ObjectUIInfoLabel *UIInfoLabel;

// Standard Entity Events
void UIInfoLabel_Update(void);
void UIInfoLabel_LateUpdate(void);
void UIInfoLabel_StaticUpdate(void);
void UIInfoLabel_Draw(void);
void UIInfoLabel_Create(void *data);
void UIInfoLabel_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UIInfoLabel_EditorDraw(void);
void UIInfoLabel_EditorLoad(void);
#endif
void UIInfoLabel_Serialize(void);

// Extra Entity Functions
void UIInfoLabel_SetText(EntityUIInfoLabel *label, char *text);
void UIInfoLabel_SetString(EntityUIInfoLabel *self, String *text);
void UIInfoLabel_DrawSprites(void);

#endif //! OBJ_UIINFOLABEL_H
