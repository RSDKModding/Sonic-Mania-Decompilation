#ifndef OBJ_UIVSCHARSELECTOR_H
#define OBJ_UIVSCHARSELECTOR_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectUIVsCharSelector;

// Entity Class
typedef struct {
	MANIA_UI_ITEM_BASE
    uint8 playerID;
    bool32 prevFlag;
    int32 frameID;
    int32 field_110;
    int32 field_114;
    int32 field_118;
    int32 field_11C;
    bool32 ready;
    int32 field_124;
    int32 field_128;
    int32 field_12C;
    int32 field_130;
    int32 field_134;
    int32 field_138;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    Animator animator5;
    Animator animator6;
    uint16 textSpriteIndex;
    int32 prevFrameID;
} EntityUIVsCharSelector;

// Object Struct
extern ObjectUIVsCharSelector *UIVsCharSelector;

// Standard Entity Events
void UIVsCharSelector_Update(void);
void UIVsCharSelector_LateUpdate(void);
void UIVsCharSelector_StaticUpdate(void);
void UIVsCharSelector_Draw(void);
void UIVsCharSelector_Create(void* data);
void UIVsCharSelector_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void UIVsCharSelector_EditorDraw(void);
void UIVsCharSelector_EditorLoad(void);
#endif
void UIVsCharSelector_Serialize(void);

// Extra Entity Functions
void UIVsCharSelector_SetupText(void);
void UIVsCharSelector_Unknown2(void);
void UIVsCharSelector_Unknown3(void);
void UIVsCharSelector_Unknown4(void);
void UIVsCharSelector_ProcessButtonCB(void);
void UIVsCharSelector_ProcessButtonCB_Alt(void);
void UIVsCharSelector_Unknown7(void);
void UIVsCharSelector_Unknown8(void);
void UIVsCharSelector_Unknown9(void);
#if RETRO_USE_PLUS
void UIVsCharSelector_Unknown10(void);
#endif
void UIVsCharSelector_Unknown11(void);

#endif //!OBJ_UIVSCHARSELECTOR_H
