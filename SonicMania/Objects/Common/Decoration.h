#ifndef OBJ_DECORATION_H
#define OBJ_DECORATION_H

#include "Game.h"

typedef enum {
    DECORATION_NONE,
} DecorationTypes;

// Object Class
struct ObjectDecoration {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityDecoration {
    RSDK_ENTITY
    Animator animator;
    uint8 type;
    int32 rotSpeed;
    Vector2 repeatTimes;
    Vector2 repeatSpacing;
    bool32 additive;
};

// Object Struct
extern ObjectDecoration *Decoration;

// Standard Entity Events
void Decoration_Update(void);
void Decoration_LateUpdate(void);
void Decoration_StaticUpdate(void);
void Decoration_Draw(void);
void Decoration_Create(void *data);
void Decoration_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void Decoration_EditorDraw(void);
void Decoration_EditorLoad(void);
#endif
void Decoration_Serialize(void);

// Extra Entity Functions
void Decoration_DrawSprite(void);

#endif //! OBJ_DECORATION_H
