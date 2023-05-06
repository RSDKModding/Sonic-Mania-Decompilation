#ifndef OBJ_UIOPTIONPANEL_H
#define OBJ_UIOPTIONPANEL_H

#include "Game.h"

// Object Class
struct ObjectUIOptionPanel {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityUIOptionPanel {
    RSDK_ENTITY
    int32 panelSize;
    int32 topListID;
    int32 topFrameID;
    int32 botListID;
    int32 botFrameID;
    bool32 botAlignRight;
    bool32 botHidden;
    Vector2 startPos;
    Vector2 size;
    Vector2 drawPosTop;
    Vector2 drawPosBottom;
    Hitbox hitbox;
    Animator topAnimator;
    Animator botAnimator;
    uint16 textFrames;
};

// Object Struct
extern ObjectUIOptionPanel *UIOptionPanel;

// Standard Entity Events
void UIOptionPanel_Update(void);
void UIOptionPanel_LateUpdate(void);
void UIOptionPanel_StaticUpdate(void);
void UIOptionPanel_Draw(void);
void UIOptionPanel_Create(void *data);
void UIOptionPanel_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UIOptionPanel_EditorDraw(void);
void UIOptionPanel_EditorLoad(void);
#endif
void UIOptionPanel_Serialize(void);

// Extra Entity Functions
void UIOptionPanel_DrawBG(void);

#endif //! OBJ_UIOPTIONPANEL_H
