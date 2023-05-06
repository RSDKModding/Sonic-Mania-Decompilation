#ifndef OBJ_UISUBHEADING_H
#define OBJ_UISUBHEADING_H

#include "Game.h"

// Object Class
struct ObjectUISubHeading {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityUISubHeading {
    RSDK_ENTITY
    StateMachine(state);
    int32 unused1;
    int32 unused2;
    int32 unused3;
    int32 unused4;
    Vector2 size;
    int32 listID;
    int32 frameID;
    int32 align;
    int32 offset;
    int32 bgEdgeSize;
    int32 storedListID;
    int32 storedFrameID;
    Animator animator;
    uint16 textFrames;
};

// Object Struct
extern ObjectUISubHeading *UISubHeading;

// Standard Entity Events
void UISubHeading_Update(void);
void UISubHeading_LateUpdate(void);
void UISubHeading_StaticUpdate(void);
void UISubHeading_Draw(void);
void UISubHeading_Create(void *data);
void UISubHeading_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UISubHeading_EditorDraw(void);
void UISubHeading_EditorLoad(void);
#endif
void UISubHeading_Serialize(void);

// Extra Entity Functions
#if MANIA_USE_PLUS
void UISubHeading_Initialize(void);
void UISubHeading_HandleUnlocks(void);
void UISubHeading_SetupActions(void);
void UISubHeading_HandleMenuReturn(int32 slot);
int32 UISubHeading_GetMedalMods(void);
void UISubHeading_SaveFileCB(bool32 success);
void UISubHeading_SecretsTransitionCB(void);
void UISubHeading_LeaveSecretsMenu(void);

void UISubHeading_SaveSel_MenuUpdateCB(void);
void UISubHeading_SaveSel_YPressCB(void);
void UISubHeading_SaveButton_ActionCB(void);
#endif

#endif //! OBJ_UISUBHEADING_H
