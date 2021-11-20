#ifndef OBJ_UIVSSCOREBOARD_H
#define OBJ_UIVSSCOREBOARD_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectUIVsScoreboard;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int32 field_58;
    int32 field_5C;
    int32 field_60;
    int32 field_64;
    int32 field_68;
    int32 field_6C;
    Vector2 *posPtr;
    EntityUIControl *parent;
    Vector2 offset;
    Vector2 size;
    uint8 p1Score;
    uint8 p2Score;
    bool32 showWinner;
    uint8 winnerID;
    int32 sizeY;
    TextInfo scoreText;
    Animator animator1;
    Animator textAnimator;
    Animator animator2;
    Animator animator3;
    uint16 textSpriteIndex;
} EntityUIVsScoreboard;

// Object Struct
extern ObjectUIVsScoreboard *UIVsScoreboard;

// Standard Entity Events
void UIVsScoreboard_Update(void);
void UIVsScoreboard_LateUpdate(void);
void UIVsScoreboard_StaticUpdate(void);
void UIVsScoreboard_Draw(void);
void UIVsScoreboard_Create(void* data);
void UIVsScoreboard_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void UIVsScoreboard_EditorDraw(void);
void UIVsScoreboard_EditorLoad(void);
#endif
void UIVsScoreboard_Serialize(void);

// Extra Entity Functions
void UIVsScoreboard_SetupSprites(void);
void UIVsScoreboard_SetText(EntityUIVsScoreboard *self, const char *text);
void UIVsScoreboard_DrawSprites(void);

#endif //!OBJ_UIVSSCOREBOARD_H
