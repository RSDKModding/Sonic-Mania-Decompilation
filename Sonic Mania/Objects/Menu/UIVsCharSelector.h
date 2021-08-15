#ifndef OBJ_UIVSCHARSELECTOR_H
#define OBJ_UIVSCHARSELECTOR_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort aniFrames;
} ObjectUIVsCharSelector;

// Entity Class
typedef struct {
	MANIA_UI_ITEM_BASE
    byte playerID;
    bool32 prevFlag;
    int frameID;
    int field_110;
    int field_114;
    int field_118;
    int field_11C;
    bool32 ready;
    int field_124;
    int field_128;
    int field_12C;
    int field_130;
    int field_134;
    int field_138;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    Animator animator4;
    Animator animator5;
    Animator animator6;
    ushort textSpriteIndex;
    int prevFrameID;
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
void UIVsCharSelector_EditorDraw(void);
void UIVsCharSelector_EditorLoad(void);
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
void UIVsCharSelector_Unknown10(void);
void UIVsCharSelector_Unknown11(void);

#endif //!OBJ_UIVSCHARSELECTOR_H
