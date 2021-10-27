#ifndef OBJ_UIKEYBINDER_H
#define OBJ_UIKEYBINDER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxFail;
    Entity *activeBinder;
    int32 curInputID;
    int32 curMappings;
    bool32 flag;
} ObjectUIKeyBinder;

// Entity Class
typedef struct {
    MANIA_UI_ITEM_BASE
    uint8 type;
    uint8 inputID;
    int32 listID;
    int32 frameID;
    int32 field_110;
    int32 field_114;
    int32 field_118;
    int32 field_11C;
    int32 field_120;
    int32 field_124;
    int32 field_128;
    int32 field_12C;
    bool32 textFlag;
    int32 field_134;
    Animator animator1;
    Animator animator2;
    uint16 textSpriteIndex;
    int32 field_16C;
} EntityUIKeyBinder;

// Object Struct
extern ObjectUIKeyBinder *UIKeyBinder;

// Standard Entity Events
void UIKeyBinder_Update(void);
void UIKeyBinder_LateUpdate(void);
void UIKeyBinder_StaticUpdate(void);
void UIKeyBinder_Draw(void);
void UIKeyBinder_Create(void* data);
void UIKeyBinder_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void UIKeyBinder_EditorDraw(void);
void UIKeyBinder_EditorLoad(void);
#endif
void UIKeyBinder_Serialize(void);

// Extra Entity Functions
int32 UIKeyBinder_GetButtonListID(void);
int32 UIKeyBinder_GetMappings(int32 input, int32 button);
void UIKeyBinder_SetMappings(int32 input, int32 button, int32 mappings);
int32 UIKeyBinder_Unknown4(int32 id);
void UIKeyBinder_Unknown5(void);
void UIKeyBinder_Options2CB(void);
bool32 UIKeyBinder_Options7CB(void);
bool32 UIKeyBinder_Options8CB(void);
void UIKeyBinder_Options5CB(void);
void UIKeyBinder_Options6CB(void);
void UIKeyBinder_Options3CB(void);
void UIKeyBinder_Unknown12(void);
void UIKeyBinder_Unknown13(void);
void UIKeyBinder_Unknown14(void);
void UIKeyBinder_Unknown15(void);
void UIKeyBinder_Unknown16(void);

#endif //!OBJ_UIKEYBINDER_H
