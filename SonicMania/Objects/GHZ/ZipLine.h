#ifndef OBJ_ZIPLINE_H
#define OBJ_ZIPLINE_H

#include "Game.h"

// Object Class
struct ObjectZipLine {
    RSDK_OBJECT
    uint16 aniFrames;
    Hitbox hitboxHandle;
};

// Entity Class
struct EntityZipLine {
    RSDK_ENTITY
    StateMachine(state);
    uint8 activePlayers;
    Animator animator;
    int32 length;
    Vector2 startPos;
    Vector2 endPos;
    Vector2 handlePos;
    uint8 grabDelay[PLAYER_COUNT];
    Vector2 joinPos;
    int32 unused1;
};

// Object Struct
extern ObjectZipLine *ZipLine;

// Standard Entity Events
void ZipLine_Update(void);
void ZipLine_LateUpdate(void);
void ZipLine_StaticUpdate(void);
void ZipLine_Draw(void);
void ZipLine_Create(void *data);
void ZipLine_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void ZipLine_EditorDraw(void);
void ZipLine_EditorLoad(void);
#endif
void ZipLine_Serialize(void);

// Extra Entity Functions
void ZipLine_VSSwap_CheckBusy(void);
void ZipLine_GrabHandle(EntityPlayer *player, int32 playerID, Hitbox *playerHitbox);
void ZipLine_ForceReleasePlayers(void);
Vector2 ZipLine_GetJoinPos(void);
void ZipLine_State_Moving(void);

#endif //! OBJ_ZIPLINE_H
