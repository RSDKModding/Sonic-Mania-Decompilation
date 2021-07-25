#ifndef OBJ_UIBUTTONPROMPT_H
#define OBJ_UIBUTTONPROMPT_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    bool32 field_4;
    int inputID;
    ushort aniFrames;
} ObjectUIButtonPrompt;

// Entity Class
typedef struct {
    RSDK_ENTITY
    Entity *parent;
    StateMachine(state);
    int timer;
    Vector2 startPos;
    int promptID;
    int buttonID;
    byte headingAnchor;
    int field_78;
    int prevPrompt;
    int prevButton;
    int mappings;
    bool32 flag;
    int scaleMax;
    int scaleSpeed;
    int field_94;
    Vector2 touchSize;
    Vector2 touchPos;
    bool32 touched;
    Animator animator1;
    Animator animator2;
    Animator animator3;
    ushort textSprite;
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
void UIButtonPrompt_EditorDraw(void);
void UIButtonPrompt_EditorLoad(void);
void UIButtonPrompt_Serialize(void);

// Extra Entity Functions
int UIButtonPrompt_GetButtonMappings(int input, int button);
int UIButtonPrompt_GetGamepadType(void);
byte UIButtonPrompt_MappingsToFrame(int mappings);

bool32 UIButtonPrompt_CheckTouch(void);
void UIButtonPrompt_Unknown4(void);
void UIButtonPrompt_Unknown6(void);
void UIButtonPrompt_Unknown7(void);

#endif //!OBJ_UIBUTTONPROMPT_H
