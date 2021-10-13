#ifndef OBJ_UISLIDER_H
#define OBJ_UISLIDER_H

#include "SonicMania.h"

#define UISlider_MinVal (0)
#define UISlider_MaxVal (0x400)
#define UISlider_Increment (UISlider_MaxVal / 0x10)

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 field_4;
    Entity *entityPtr;
} ObjectUISlider;

// Entity Class
typedef struct {
	MANIA_UI_ITEM_BASE
    int32 listID;
    int32 frameID;
    int32 field_10C;
    int32 field_110;
    int32 field_114;
    int32 field_118;
    int32 field_11C;
    int32 field_120;
    int32 field_124;
    int32 sliderPos;
    bool32 textFlag;
    bool32 flagB;
    int32 field_134;
    void(*sliderChangedCB)(void);
    Animator textAnimator;
    uint16 textSpriteIndex;
} EntityUISlider;

// Object Struct
extern ObjectUISlider *UISlider;

// Standard Entity Events
void UISlider_Update(void);
void UISlider_LateUpdate(void);
void UISlider_StaticUpdate(void);
void UISlider_Draw(void);
void UISlider_Create(void* data);
void UISlider_StageLoad(void);
void UISlider_EditorDraw(void);
void UISlider_EditorLoad(void);
void UISlider_Serialize(void);

// Extra Entity Functions
void UISlider_Unknown1(void);
void UISlider_Unknown2(void);

void UISlider_ButtonPressCB(void);
bool32 UISlider_TouchCB(void);
void UISlider_Options5CB(void);
void UISlider_Options6CB(void);
bool32 UISlider_Options7CB(void);
bool32 UISlider_Options8CB(void);

void UISlider_Unknown9(void);
void UISlider_Unknown10(void);

#endif //!OBJ_UISLIDER_H
