#ifndef OBJ_HANGCONVEYOR_H
#define OBJ_HANGCONVEYOR_H

#include "Game.h"

// Object Class
struct ObjectHangConveyor {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityHangConveyor {
    RSDK_ENTITY
    StateMachine(state);
    int32 length;
    uint8 movementActivePlayers;
    uint8 activePlayersTop;
    uint8 activePlayersBottom;
    uint8 checkableActivePlayers;
    Vector2 endPosTopLeft;
    Vector2 endPosTopRight;
    Vector2 endPosBottomLeft;
    Vector2 endPosBottomRight;
    int32 grabDelayTop[PLAYER_COUNT];
    int32 grabDelayBottom[PLAYER_COUNT];
    Vector2 playerPositions[PLAYER_COUNT];
    int32 fanTimer[PLAYER_COUNT];
    Vector2 startPos;
    int32 unused;
    Hitbox hitboxFallCheckTop;
    Hitbox hitboxFallCheckBottom;
    Hitbox hitboxSpikeBallRange;
    Animator endAnimator;
    Animator startAnimator;
    Animator middleAnimator;
};

// Object Struct
extern ObjectHangConveyor *HangConveyor;

// Standard Entity Events
void HangConveyor_Update(void);
void HangConveyor_LateUpdate(void);
void HangConveyor_StaticUpdate(void);
void HangConveyor_Draw(void);
void HangConveyor_Create(void *data);
void HangConveyor_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void HangConveyor_EditorDraw(void);
void HangConveyor_EditorLoad(void);
#endif
void HangConveyor_Serialize(void);

// Extra Entity Functions
void HangConveyor_DrawSprites(void);

void HangConveyor_SetupHitboxes(void);
void HangConveyor_HandlePlayerInteractions(void);

#endif //! OBJ_HANGCONVEYOR_H
