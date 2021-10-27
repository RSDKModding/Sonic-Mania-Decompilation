#ifndef OBJ_UICHOICE_H
#define OBJ_UICHOICE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectUIChoice;

// Entity Class
typedef struct {
    MANIA_UI_ITEM_BASE
    Vector2 size;
    int32 listID;
    int32 frameID;
    bool32 noText;
    bool32 auxIcon;
    int32 auxListID;
    int32 auxFrameID;
    int32 align;
    int32 arrowWidth;
    bool32 isDisabled;
    int32 sizeIY;
    Vector2 field_134;
    int32 field_13C;
    int32 field_140;
    bool32 textFlag;
    int32 field_148;
    int32 touchID;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    uint16 spriteIndex;
} EntityUIChoice;

// Object Struct
extern ObjectUIChoice *UIChoice;

// Standard Entity Events
void UIChoice_Update(void);
void UIChoice_LateUpdate(void);
void UIChoice_StaticUpdate(void);
void UIChoice_Draw(void);
void UIChoice_Create(void* data);
void UIChoice_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void UIChoice_EditorDraw(void);
void UIChoice_EditorLoad(void);
#endif
void UIChoice_Serialize(void);

// Extra Entity Functions
void UIChoice_Unknown1(EntityUIChoice *entity);
void UIChoice_Unknown2(EntityUIButton *entity);
void UIChoice_TouchedCB_Left(void);
void UIChoice_TouchedCB_Right(void);
bool32 UIChoice_CheckTouch(void);
void UIChoice_Unknown6(void);
void UIChoice_Unknown7(void);

#endif //!OBJ_UICHOICE_H
