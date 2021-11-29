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
    int32 panelSize;
    int32 topListID;
    int32 topFrameID;
    int32 botListID;
    int32 botFrameID;
    bool32 botAlignRight;
    bool32 botHidden;
    int32 field_74;
    int32 field_78;
    Vector2 field_7C;
    Vector2 drawPosTop;
    Vector2 drawPosBottom;
    int32 field_94;
    int32 field_98;
    Animator animator;
    Animator animator2;
    uint16 textFrames;
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
#if RETRO_INCLUDE_EDITOR
void UIOptionPanel_EditorDraw(void);
void UIOptionPanel_EditorLoad(void);
#endif
void UIOptionPanel_Serialize(void);

// Extra Entity Functions
void UIOptionPanel_Unknown1(void);

#endif //!OBJ_UIOPTIONPANEL_H
