#ifndef OBJ_UICAROUSEL_H
#define OBJ_UICAROUSEL_H

#include "Game.h"

#if MANIA_USE_PLUS
// Object Class
struct ObjectUICarousel {
    RSDK_OBJECT
    uint16 aniFrames;  // both of these aren't used, and are thus prolly editor-only
    Animator animator; // both of these aren't used, and are thus prolly editor-only
};

// Entity Class
struct EntityUICarousel {
    RSDK_ENTITY
    Vector2 shift;
    Vector2 clipSize;
    Vector2 clipOffset;
    int32 unused1;
    int32 unused2;
    int32 unused3;
    Entity *unused4;
    int32 unused5;
    int32 scrollOffset;
    int32 virtualIndex;
    int32 unused6;
    Vector2 startPos;
    int32 buttonID;
    int32 minOffset;
    int32 maxOffset;
    Vector2 offset;
    EntityUIControl *parent;
};

// Object Struct
extern ObjectUICarousel *UICarousel;

// Standard Entity Events
void UICarousel_Update(void);
void UICarousel_LateUpdate(void);
void UICarousel_StaticUpdate(void);
void UICarousel_Draw(void);
void UICarousel_Create(void *data);
void UICarousel_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void UICarousel_EditorDraw(void);
void UICarousel_EditorLoad(void);
#endif
void UICarousel_Serialize(void);

// Extra Entity Functions
void UICarousel_HandleScrolling(void);
void UICarousel_HandleButtonPositions(void);

#endif

#endif //! OBJ_UICAROUSEL_H
