#ifndef OBJ_UIBUTTONPROMPT_H
#define OBJ_UIBUTTONPROMPT_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    int32 type;
    int32 inputID;
    uint16 aniFrames;
} ObjectUIButtonPrompt;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Entity *parent;
    StateMachine(state);
    int32 timer;
    Vector2 startPos;
    int32 promptID;
    int32 buttonID;
    uint8 headingAnchor;
    int32 field_78;
    int32 prevPrompt;
    int32 prevButton;
    int32 mappings;
    bool32 flag;
    int32 scaleMax;
    int32 scaleSpeed;
    int32 field_94;
    Vector2 touchSize;
    Vector2 touchPos;
    bool32 touched;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    uint16 textSprite;
} EntityUIButtonPrompt;

// Object Struct
extern ObjectUIButtonPrompt *UIButtonPrompt;

// Standard Entity Events
void UIButtonPrompt_Update(void);
void UIButtonPrompt_LateUpdate(void);
void UIButtonPrompt_StaticUpdate(void);
void UIButtonPrompt_Draw(void);
void UIButtonPrompt_Create(void* data);
void UIButtonPrompt_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void UIButtonPrompt_EditorDraw(void);
void UIButtonPrompt_EditorLoad(void);
#endif
void UIButtonPrompt_Serialize(void);

// Extra Entity Functions
int32 UIButtonPrompt_GetButtonMappings(int32 input, int32 button);
int32 UIButtonPrompt_GetGamepadType(void);
uint8 UIButtonPrompt_MappingsToFrame(int32 mappings);

bool32 UIButtonPrompt_CheckTouch(void);
void UIButtonPrompt_Unknown4(void);
void UIButtonPrompt_Unknown6(void);
void UIButtonPrompt_Unknown7(void);

#endif //!OBJ_UIBUTTONPROMPT_H
