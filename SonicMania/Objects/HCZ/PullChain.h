#ifndef OBJ_PULLCHAIN_H
#define OBJ_PULLCHAIN_H

#include "Game.h"

typedef enum {
    PULLCHAIN_NORMAL,
} PullChainTypes;

typedef enum {
    PULLCHAIN_INPUT_NONE,
    PULLCHAIN_INPUT_LEFT,
    PULLCHAIN_INPUT_RIGHT,
    PULLCHAIN_INPUT_UP,
    PULLCHAIN_INPUT_DOWN,
} PullChainInputMasks;

// Object Class
struct ObjectPullChain {
    RSDK_OBJECT
    // left, left, left, right, right, right, up, up, up!
    TABLE(int32 dunkeyCode[18], { 0, 1, 0, 1, 0, 1, 0, 2, 0, 2, 0, 2, 0, 3, 0, 3, 0, 3 });
    uint16 aniFrames;
    uint16 sfxPullChain;
};

// Entity Class
struct EntityPullChain {
    MANIA_BUTTON_BASE

    Animator hookAnimator;
    int32 length;
    int32 decorMode;
    uint8 activePlayers;
    uint8 releasedPlayers;
    Vector2 basePos;
    int32 unused;
    int32 chainOffset;
    int32 grabDelay[PLAYER_COUNT];
    uint8 codeButtonMasks;
    int32 cheatCodeInputs[18];
    Hitbox hitbox;
    Animator chainAnimator;
};

// Object Struct
extern ObjectPullChain *PullChain;

// Standard Entity Events
void PullChain_Update(void);
void PullChain_LateUpdate(void);
void PullChain_StaticUpdate(void);
void PullChain_Draw(void);
void PullChain_Create(void *data);
void PullChain_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PullChain_EditorDraw(void);
void PullChain_EditorLoad(void);
#endif
void PullChain_Serialize(void);

// Extra Entity Functions
#if GAME_VERSION == VER_100
bool32 PullChain_HandleDunkeyCode(EntityPlayer *player);
#endif

#endif //! OBJ_PULLCHAIN_H
