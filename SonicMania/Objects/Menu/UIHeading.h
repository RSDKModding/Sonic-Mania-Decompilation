#ifndef OBJ_UIHEADING_H
#define OBJ_UIHEADING_H

#include "Game.h"

typedef enum {
    UIHEADING_MAINMENU,
    UIHEADING_MANIAMODE,
    UIHEADING_TIMEATTACK,
    UIHEADING_COMPETITION,
    UIHEADING_RESULTS,
    UIHEADING_OPTIONS,
    UIHEADING_EXTRAS,
    UIHEADING_LEADERBOARDS,
    UIHEADING_SECRETS,
    UIHEADING_PLAYERSEL,
    UIHEADING_ZONESEL,
    UIHEADING_REPLAYS,
    UIHEADING_ENCOREMODE,
} UIHeadingIDs;

// Object Class
struct ObjectUIHeading {
    RSDK_OBJECT
    uint16 textFrames;
};

// Entity Class
struct EntityUIHeading {
    RSDK_ENTITY
    StateMachine(state);
    int32 unused;
    Vector2 startPos;
    int32 headingID;
    Animator headingAnimator;
    Animator textAnimator;
    uint16 textFrames;
};

// Object Struct
extern ObjectUIHeading *UIHeading;

// Standard Entity Events
void UIHeading_Update(void);
void UIHeading_LateUpdate(void);
void UIHeading_StaticUpdate(void);
void UIHeading_Draw(void);
void UIHeading_Create(void *data);
void UIHeading_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UIHeading_EditorDraw(void);
void UIHeading_EditorLoad(void);
#endif
void UIHeading_Serialize(void);

// Extra Entity Functions
void UIHeading_LoadSprites(void);

#endif //! OBJ_UIHEADING_H
