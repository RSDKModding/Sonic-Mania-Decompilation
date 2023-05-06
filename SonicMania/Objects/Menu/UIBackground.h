#ifndef OBJ_UIBACKGROUND_H
#define OBJ_UIBACKGROUND_H

#include "Game.h"

typedef enum { UIBACKGROUND_UNUSED } UIBackgroundTypes;

// Object Class
struct ObjectUIBackground {
    RSDK_OBJECT
#if MANIA_USE_PLUS
    TABLE(color bgColors[21], { 0xF0C800, 0xF08C18, 0x80A0B0, 0x50D8A0, 0xF09018, 0x38B0C8, 0x38B0C8, 0xF07028, 0x50D8A0, 0xC83868, 0x50D8A0,
                                0xF09018, 0xC0C098, 0x38B0C8, 0x88A0B8, 0xA850F0, 0x88C801, 0x1B0F0,  0x50B098, 0x890C8,  0xE8B010 });
#else
    TABLE(color bgColors[15], { 0xF0C800, 0xF08C18, 0x80A0B0, 0x50D8A0, 0xF09018, 0x38B0C8, 0x38B0C8, 0xF07028, 0x50D8A0, 0xC83868, 0x50D8A0,
                                0xF09018, 0xC0C098, 0x38B0C8, 0x88A0B8 });
#endif
    color *activeColors;
    uint16 aniFrames;
};

// Entity Class
struct EntityUIBackground {
    RSDK_ENTITY
    StateMachine(stateDraw);
    UIBackgroundTypes type;
    int32 timer;
    Animator animator; // probably editor-only
};

// Object Struct
extern ObjectUIBackground *UIBackground;

// Standard Entity Events
void UIBackground_Update(void);
void UIBackground_LateUpdate(void);
void UIBackground_StaticUpdate(void);
void UIBackground_Draw(void);
void UIBackground_Create(void *data);
void UIBackground_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UIBackground_EditorDraw(void);
void UIBackground_EditorLoad(void);
#endif
void UIBackground_Serialize(void);

// Extra Entity Functions
void UIBackground_DrawNormal(void);

#endif //! OBJ_UIBACKGROUND_H
