#ifndef OBJ_UIBUTTONLABEL_H
#define OBJ_UIBUTTONLABEL_H

#include "Game.h"

// Object Class
struct ObjectUIButtonLabel {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityUIButtonLabel {
    RSDK_ENTITY
    int32 unused1;
    int32 unused2;
    int32 unused3;
    int32 unused4;
    uint8 type;
    int32 listID;
    int32 frameID;
    Animator frameAnimator;
    Animator textAnimator;
    uint16 textSprite;
};

// Object Struct
extern ObjectUIButtonLabel *UIButtonLabel;

// Standard Entity Events
void UIButtonLabel_Update(void);
void UIButtonLabel_LateUpdate(void);
void UIButtonLabel_StaticUpdate(void);
void UIButtonLabel_Draw(void);
void UIButtonLabel_Create(void *data);
void UIButtonLabel_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UIButtonLabel_EditorDraw(void);
void UIButtonLabel_EditorLoad(void);
#endif
void UIButtonLabel_Serialize(void);

// Extra Entity Functions

#endif //! OBJ_UIBUTTONLABEL_H
