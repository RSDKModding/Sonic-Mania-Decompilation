#ifndef OBJ_UIBUTTONPROMPT_H
#define OBJ_UIBUTTONPROMPT_H

#include "Game.h"

typedef enum {
    UIBUTTONPROMPT_NONE,
    UIBUTTONPROMPT_KEYBOARD,
    UIBUTTONPROMPT_XBOX,
    UIBUTTONPROMPT_PS4,
    UIBUTTONPROMPT_SWITCH,
    UIBUTTONPROMPT_SATURN_BLACK,
    UIBUTTONPROMPT_SATURN_WHITE,
    UIBUTTONPROMPT_JOYCON_L,
    UIBUTTONPROMPT_JOYCON_R,
    UIBUTTONPROMPT_KEYBOARD_FR,
    UIBUTTONPROMPT_KEYBOARD_IT,
    UIBUTTONPROMPT_KEYBOARD_GE,
    UIBUTTONPROMPT_KEYBOARD_SP,
} UIButtonPromptTypes;

typedef enum {
    UIBUTTONPROMPT_ANCHOR_NONE,
    UIBUTTONPROMPT_ANCHOR_TOPLEFT_ROW1,
    UIBUTTONPROMPT_ANCHOR_TOPRIGHT_ROW1,
    UIBUTTONPROMPT_ANCHOR_TOPLEFT_ROW2,
    UIBUTTONPROMPT_ANCHOR_TOPRIGHT_ROW2,
} UIButtonPromptAnchors;

typedef enum {
    UIBUTTONPROMPT_BUTTON_A,
    UIBUTTONPROMPT_BUTTON_B,
    UIBUTTONPROMPT_BUTTON_X,
    UIBUTTONPROMPT_BUTTON_Y,
    UIBUTTONPROMPT_BUTTON_START,
    UIBUTTONPROMPT_BUTTON_SELECT,
} UIButtonPromptButtons;

// Object Class
struct ObjectUIButtonPrompt {
    RSDK_OBJECT
    int32 type;
    int32 inputSlot;
    uint16 aniFrames;
};

// Entity Class
struct EntityUIButtonPrompt {
    RSDK_ENTITY
    Entity *parent;
    StateMachine(state);
    int32 timer;
    Vector2 startPos;
    int32 promptID;
    int32 buttonID;
    uint8 headingAnchor;
    int32 unused;
    int32 prevPrompt;
    int32 prevButton;
    int32 mappings;
    bool32 textVisible;
    int32 scaleMax;
    int32 scaleSpeed;
    bool32 disableScale;
    Vector2 touchSize;
    Vector2 touchPos;
    bool32 touched;
    Animator decorAnimator;
    Animator buttonAnimator;
    Animator promptAnimator;
    uint16 textSprite;
};

// Object Struct
extern ObjectUIButtonPrompt *UIButtonPrompt;

// Standard Entity Events
void UIButtonPrompt_Update(void);
void UIButtonPrompt_LateUpdate(void);
void UIButtonPrompt_StaticUpdate(void);
void UIButtonPrompt_Draw(void);
void UIButtonPrompt_Create(void *data);
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
void UIButtonPrompt_SetButtonSprites(void);
void UIButtonPrompt_State_CheckIfSelected(void);
void UIButtonPrompt_State_Selected(void);

#endif //! OBJ_UIBUTTONPROMPT_H
