#ifndef OBJ_UIKEYBINDER_H
#define OBJ_UIKEYBINDER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort aniFrames;
    ushort sfxFail;
    Entity *activeBinder;
    int curInputID;
    int curMappings;
    bool32 flag;
} ObjectUIKeyBinder;

// Entity Class
typedef struct {
    MANIA_UI_ITEM_BASE
    byte type;
    byte inputID;
    int listID;
    int frameID;
    int field_110;
    int field_114;
    int field_118;
    int field_11C;
    int field_120;
    int field_124;
    int field_128;
    int field_12C;
    bool32 textFlag;
    int field_134;
    Animator animator1;
    Animator animator2;
    ushort textSpriteIndex;
    int field_16C;
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
void UIKeyBinder_EditorDraw(void);
void UIKeyBinder_EditorLoad(void);
void UIKeyBinder_Serialize(void);

// Extra Entity Functions
int UIKeyBinder_GetButtonListID(void);
int UIKeyBinder_GetMappings(int input, int button);
void UIKeyBinder_SetMappings(int input, int button, int mappings);
int UIKeyBinder_Unknown4(int id);
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
