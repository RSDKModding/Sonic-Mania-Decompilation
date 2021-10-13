#ifndef OBJ_UIOPTIONPANEL_H
#define OBJ_UIOPTIONPANEL_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectUIOptionPanel;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int panelSize;
    int topListID;
    int topFrameID;
    int botListID;
    int botFrameID;
    bool32 botAlignRight;
    bool32 botHidden;
    int field_74;
    int field_78;
    Vector2 field_7C;
    Vector2 drawPosTop;
    Vector2 drawPosBottom;
    int field_94;
    int field_98;
    Animator animator;
    Animator animator2;
    uint16 textSpriteIndex;
} EntityUIOptionPanel;

// Object Struct
extern ObjectUIOptionPanel *UIOptionPanel;

// Standard Entity Events
void UIOptionPanel_Update(void);
void UIOptionPanel_LateUpdate(void);
void UIOptionPanel_StaticUpdate(void);
void UIOptionPanel_Draw(void);
void UIOptionPanel_Create(void* data);
void UIOptionPanel_StageLoad(void);
void UIOptionPanel_EditorDraw(void);
void UIOptionPanel_EditorLoad(void);
void UIOptionPanel_Serialize(void);

// Extra Entity Functions
void UIOptionPanel_Unknown1(void);

#endif //!OBJ_UIOPTIONPANEL_H
