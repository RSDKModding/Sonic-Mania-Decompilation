#ifndef OBJ_UISHIFTER_H
#define OBJ_UISHIFTER_H

#include "Game.h"

#if MANIA_USE_PLUS
// Object Class
struct ObjectUIShifter {
    RSDK_OBJECT
    Animator animator;
    uint16 aniFrames;
};

// Entity Class
struct EntityUIShifter {
    RSDK_ENTITY
    Vector2 shift;
    Vector2 lastButtonShift;
    Vector2 shiftOffset;
    int32 unused;
    Vector2 startPos;
    EntityUIControl *parent;
};

// Object Struct
extern ObjectUIShifter *UIShifter;

// Standard Entity Events
void UIShifter_Update(void);
void UIShifter_LateUpdate(void);
void UIShifter_StaticUpdate(void);
void UIShifter_Draw(void);
void UIShifter_Create(void *data);
void UIShifter_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UIShifter_EditorDraw(void);
void UIShifter_EditorLoad(void);
#endif
void UIShifter_Serialize(void);

// Extra Entity Functions
void UIShifter_HandleShift(void);
#endif

#endif //! OBJ_UISHIFTER_H
