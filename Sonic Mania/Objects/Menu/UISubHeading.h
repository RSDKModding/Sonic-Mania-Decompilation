#ifndef OBJ_UISUBHEADING_H
#define OBJ_UISUBHEADING_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort aniFrames;
} ObjectUISubHeading;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    int field_5C;
    int field_60;
    int field_64;
    int field_68;
    Vector2 size;
    int listID;
    int frameID;
    int align;
    int offset;
    int shiftedY;
    int storedListID;
    int storedFrameID;
    Animator animator;
    ushort textSpriteIndex;
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
void UISubHeading_Initialize(void);
void UISubHeading_Unknown2(void);
void UISubHeading_Unknown3(void);
void UISubHeading_Unknown4(int slot);
int UISubHeading_GetMedalMods(void);
void UISubHeading_SaveFileCB(int status);
void UISubHeading_SecretsTransitionCB(void);
void UISubHeading_Unknown9(void);
void UISubHeading_Unknown10(void);
void UISubHeading_Unknown11(void);
void UISubHeading_StartNewSave(void);

#endif //!OBJ_UISUBHEADING_H
