#ifndef OBJ_UITRANSITION_H
#define OBJ_UITRANSITION_H

#include "Game.h"

// Object Class
struct ObjectUITransition {
    RSDK_OBJECT
    Entity *activeTransition;
    char *newTag;
};

// Entity Class
struct EntityUITransition {
    RSDK_ENTITY
    StateMachine(state);
    StateMachine(callback);
    Entity *prevEntity;
    int32 timer;
    int32 delay;
    bool32 isTransitioning;
    Vector2 drawPos[3];
};

// Object Struct
extern ObjectUITransition *UITransition;

// Standard Entity Events
void UITransition_Update(void);
void UITransition_LateUpdate(void);
void UITransition_StaticUpdate(void);
void UITransition_Draw(void);
void UITransition_Create(void *data);
void UITransition_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UITransition_EditorDraw(void);
void UITransition_EditorLoad(void);
#endif
void UITransition_Serialize(void);

// Extra Entity Functions
void UITransition_StartTransition(void (*callback)(void), int32 delay);
void UITransition_MatchNewTag(void);
void UITransition_SetNewTag(const char *text);

void UITransition_DrawShapes(void);

void UITransition_State_Init(void);
void UITransition_State_TransitionIn(void);
void UITransition_State_TransitionOut(void);

#endif //! OBJ_UITRANSITION_H
