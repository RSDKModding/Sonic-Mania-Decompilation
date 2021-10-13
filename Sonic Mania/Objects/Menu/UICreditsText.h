#ifndef OBJ_UICREDITSTEXT_H
#define OBJ_UICREDITSTEXT_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectUICreditsText;

// Entity Class
typedef struct {
    RSDK_ENTITY
    StateMachine(state);
    TextInfo text;
    TextInfo tag;
    int listID;
    bool32 isHeading;
    bool32 hasShape;
    int timer;
    int scaleTimer;
    int scaleSpeed;
    Vector2 charPositions[64];
    int charOffsets[64];
    int16 charTimers[64];
    int clipY2;
    Animator animator;
} EntityUICreditsText;

// Object Struct
extern ObjectUICreditsText *UICreditsText;

// Standard Entity Events
void UICreditsText_Update(void);
void UICreditsText_LateUpdate(void);
void UICreditsText_StaticUpdate(void);
void UICreditsText_Draw(void);
void UICreditsText_Create(void* data);
void UICreditsText_StageLoad(void);
void UICreditsText_EditorDraw(void);
void UICreditsText_EditorLoad(void);
void UICreditsText_Serialize(void);

// Extra Entity Functions
void UICreditsText_SetText(int animID, EntityUICreditsText *label, TextInfo *text);

void UICreditsText_State_Setup(void);
void UICreditsText_State_SetupCharPos(void);
void UICreditsText_State_MoveChars(void);
void UICreditsText_State_ScaleIn(void);
void UICreditsText_State_FadeIn(void);
void UICreditsText_SetupIdleDelay(void);
void UICreditsText_State_Idle(void);
void UICreditsText_State_ScaleOut(void);
void UICreditsText_State_FadeOut(void);

#endif //!OBJ_UICREDITSTEXT_H
