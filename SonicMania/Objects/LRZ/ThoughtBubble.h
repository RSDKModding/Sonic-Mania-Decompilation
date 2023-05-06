#ifndef OBJ_THOUGHTBUBBLE_H
#define OBJ_THOUGHTBUBBLE_H

#include "Game.h"

typedef enum {
    THOUGHTBUBBLE_1DOT,
    THOUGHTBUBBLE_2DOTS,
    THOUGHTBUBBLE_BUBBLE,
    THOUGHTBUBBLE_THOUGHTS,
} ThoughtBubbleTypes;

// Object Class
struct ObjectThoughtBubble {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 dustFrames;
};

// Entity Class
struct EntityThoughtBubble {
    RSDK_ENTITY
    StateMachine(state);
    int32 type;
    int32 timer;
    Vector2 bubbleDotPos[2];
    Animator bubbleAnimator;
    Animator thoughtAnimator;
    Animator dotAnimator1;
    Animator dotAnimator2;
};

// Object Struct
extern ObjectThoughtBubble *ThoughtBubble;

// Standard Entity Events
void ThoughtBubble_Update(void);
void ThoughtBubble_LateUpdate(void);
void ThoughtBubble_StaticUpdate(void);
void ThoughtBubble_Draw(void);
void ThoughtBubble_Create(void *data);
void ThoughtBubble_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void ThoughtBubble_EditorDraw(void);
void ThoughtBubble_EditorLoad(void);
#endif
void ThoughtBubble_Serialize(void);

// Extra Entity Functions
void ThoughtBubble_BubbleAppear(void);
void ThoughtBubble_HaveFirstThought(void);
void ThoughtBubble_HaveOtherThoughts(void);
void ThoughtBubble_BubbleDisappear(void);
void ThoughtBubble_DestroyBubble(void);

#endif //! OBJ_THOUGHTBUBBLE_H
