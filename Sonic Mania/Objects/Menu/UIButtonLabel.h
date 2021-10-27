#ifndef OBJ_UIBUTTONLABEL_H
#define OBJ_UIBUTTONLABEL_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectUIButtonLabel;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int32 field_58;
    int32 field_5C;
    int32 field_60;
    int32 field_64;
    uint8 type;
    int32 listID;
    int32 frameID;
    Animator animator1;
    Animator animator2;
    uint16 textSprite;
} EntityUIButtonLabel;

// Object Struct
extern ObjectUIButtonLabel *UIButtonLabel;

// Standard Entity Events
void UIButtonLabel_Update(void);
void UIButtonLabel_LateUpdate(void);
void UIButtonLabel_StaticUpdate(void);
void UIButtonLabel_Draw(void);
void UIButtonLabel_Create(void* data);
void UIButtonLabel_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void UIButtonLabel_EditorDraw(void);
void UIButtonLabel_EditorLoad(void);
#endif
void UIButtonLabel_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_UIBUTTONLABEL_H
