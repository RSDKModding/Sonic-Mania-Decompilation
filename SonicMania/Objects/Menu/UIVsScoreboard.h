#ifndef OBJ_UIVSSCOREBOARD_H
#define OBJ_UIVSSCOREBOARD_H

#include "Game.h"

// Object Class
struct ObjectUIVsScoreboard {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityUIVsScoreboard {
    RSDK_ENTITY
    StateMachine(state);     // unused
    Animator unusedAnimator; // unused
    Vector2 *parentPos;
    Vector2 offset;
    Vector2 size;
    uint8 p1Score;
    uint8 p2Score;
    bool32 showWinner;
    uint8 winnerID;
    int32 bgEdgeSize;
    String scoreText;
    Animator totalAnimator;
    Animator textAnimator;
    Animator winnerAnimator;
    Animator winsAnimator;
    uint16 textFrames;
};

// Object Struct
extern ObjectUIVsScoreboard *UIVsScoreboard;

// Standard Entity Events
void UIVsScoreboard_Update(void);
void UIVsScoreboard_LateUpdate(void);
void UIVsScoreboard_StaticUpdate(void);
void UIVsScoreboard_Draw(void);
void UIVsScoreboard_Create(void *data);
void UIVsScoreboard_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UIVsScoreboard_EditorDraw(void);
void UIVsScoreboard_EditorLoad(void);
#endif
void UIVsScoreboard_Serialize(void);

// Extra Entity Functions
void UIVsScoreboard_SetupSprites(void);
void UIVsScoreboard_SetScores(EntityUIVsScoreboard *scoreboard, uint32 p1Score, uint32 p2Score);
void UIVsScoreboard_DrawSprites(void);

#endif //! OBJ_UIVSSCOREBOARD_H
