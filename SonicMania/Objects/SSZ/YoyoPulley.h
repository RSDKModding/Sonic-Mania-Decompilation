#ifndef OBJ_YOYOPULLEY_H
#define OBJ_YOYOPULLEY_H

#include "Game.h"

// Object Class
struct ObjectYoyoPulley {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitboxHandle;
};

// Entity Class
struct EntityYoyoPulley {
    RSDK_ENTITY
    uint8 activePlayers;
    Animator mainAnimator;
    Animator knobAnimator;
    Animator shineAnimator;
    Animator handleAnimator;
    uint8 pullDir;
    int32 length;
    int32 speed;
    int32 pullVelocity;
    int32 pulleyLength;
    int32 playerTimers[PLAYER_COUNT];
    Vector2 handlePos;
};

// Object Struct
extern ObjectYoyoPulley *YoyoPulley;

// Standard Entity Events
void YoyoPulley_Update(void);
void YoyoPulley_LateUpdate(void);
void YoyoPulley_StaticUpdate(void);
void YoyoPulley_Draw(void);
void YoyoPulley_Create(void *data);
void YoyoPulley_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void YoyoPulley_EditorDraw(void);
void YoyoPulley_EditorLoad(void);
#endif
void YoyoPulley_Serialize(void);

// Extra Entity Functions
void YoyoPulley_UpdateHandlePos(void);
void YoyoPulley_DrawSprites(void);
int32 YoyoPulley_GetLength(void);

#endif //! OBJ_YOYOPULLEY_H
