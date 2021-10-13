#ifndef OBJ_UISUBHEADING_H
#define OBJ_UISUBHEADING_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectUISubHeading;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int32 field_5C;
    int32 field_60;
    int32 field_64;
    int32 field_68;
    Vector2 size;
    int32 listID;
    int32 frameID;
    int32 align;
    int32 offset;
    int32 shiftedY;
    int32 storedListID;
    int32 storedFrameID;
    Animator animator;
    uint16 textSpriteIndex;
} EntityUISubHeading;

// Object Struct
extern ObjectUISubHeading *UISubHeading;

// Standard Entity Events
void UISubHeading_Update(void);
void UISubHeading_LateUpdate(void);
void UISubHeading_StaticUpdate(void);
void UISubHeading_Draw(void);
void UISubHeading_Create(void* data);
void UISubHeading_StageLoad(void);
void UISubHeading_EditorDraw(void);
void UISubHeading_EditorLoad(void);
void UISubHeading_Serialize(void);

// Extra Entity Functions
#if RETRO_USE_PLUS
void UISubHeading_Initialize(void);
void UISubHeading_Unknown2(void);
void UISubHeading_Unknown3(void);
void UISubHeading_Unknown4(int32 slot);
int32 UISubHeading_GetMedalMods(void);
void UISubHeading_SaveFileCB(int32 status);
void UISubHeading_SecretsTransitionCB(void);
void UISubHeading_Unknown9(void);
void UISubHeading_Unknown10(void);
void UISubHeading_Unknown11(void);
void UISubHeading_StartNewSave(void);
#endif

#endif //!OBJ_UISUBHEADING_H
