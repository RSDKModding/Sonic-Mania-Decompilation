#ifndef OBJ_UIVSSCOREBOARD_H
#define OBJ_UIVSSCOREBOARD_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort aniFrames;
} ObjectUIVsScoreboard;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int field_58;
    int field_5C;
    int field_60;
    int field_64;
    int field_68;
    int field_6C;
    Vector2 *posPtr;
    int field_74;
    Vector2 offset;
    Vector2 size;
    byte p1Score;
    byte p2Score;
    bool32 showWinner;
    byte winnerID;
    int sizeY;
    TextInfo scoreText;
    Animator animator1;
    Animator textAnimator;
    Animator animator2;
    Animator animator3;
    ushort textSpriteIndex;
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
void UIVsScoreboard_EditorDraw(void);
void UIVsScoreboard_EditorLoad(void);
void UIVsScoreboard_Serialize(void);

// Extra Entity Functions
void UIVsScoreboard_SetupSprites(void);
void UIVsScoreboard_SetText(EntityUIVsScoreboard *entity, const char *text);
void UIVsScoreboard_DrawSprites(void);

#endif //!OBJ_UIVSSCOREBOARD_H
