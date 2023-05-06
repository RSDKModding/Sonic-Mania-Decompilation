#ifndef OBJ_UICREDITSTEXT_H
#define OBJ_UICREDITSTEXT_H

#include "Game.h"

// Object Class
struct ObjectUICreditsText {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityUICreditsText {
    RSDK_ENTITY
    StateMachine(state);
    String text;
    String tag;
    int32 listID;
    bool32 isHeading;
    bool32 hasShape;
    int32 timer;
    int32 scaleTimer;
    int32 scaleSpeed;
    Vector2 charPositions[64];
    int32 charOffsets[64];
    int16 charTimers[64];
    int32 clipY2;
    Animator animator;
};

// Object Struct
extern ObjectUICreditsText *UICreditsText;

// Standard Entity Events
void UICreditsText_Update(void);
void UICreditsText_LateUpdate(void);
void UICreditsText_StaticUpdate(void);
void UICreditsText_Draw(void);
void UICreditsText_Create(void *data);
void UICreditsText_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UICreditsText_EditorDraw(void);
void UICreditsText_EditorLoad(void);
#endif
void UICreditsText_Serialize(void);

// Extra Entity Functions
void UICreditsText_SetText(int32 animID, EntityUICreditsText *label, String *text);

void UICreditsText_State_Init(void);
void UICreditsText_State_SetupCharPos(void);
void UICreditsText_State_MoveChars(void);
void UICreditsText_State_ScaleIn(void);
void UICreditsText_State_FadeIn(void);
void UICreditsText_SetupIdleDelay(void);
void UICreditsText_State_Idle(void);
void UICreditsText_State_ScaleOut(void);
void UICreditsText_State_FadeOut(void);

#endif //! OBJ_UICREDITSTEXT_H
