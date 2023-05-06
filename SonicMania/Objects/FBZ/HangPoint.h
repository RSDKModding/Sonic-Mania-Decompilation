#ifndef OBJ_HANGPOINT_H
#define OBJ_HANGPOINT_H

#include "Game.h"

// Object Class
struct ObjectHangPoint {
    RSDK_OBJECT
    Animator animator;
    uint16 aniFrames;
    Hitbox hitboxGrab;
};

// Entity Class
struct EntityHangPoint {
    RSDK_ENTITY
    uint8 activePlayers;
    int32 moveDistance[PLAYER_COUNT];
    int32 playerTimer[PLAYER_COUNT];
    int32 pullPos;
    int32 maxVel;
    int32 length;
    uint8 speed;
};

// Object Struct
extern ObjectHangPoint *HangPoint;

// Standard Entity Events
void HangPoint_Update(void);
void HangPoint_LateUpdate(void);
void HangPoint_StaticUpdate(void);
void HangPoint_Draw(void);
void HangPoint_Create(void *data);
void HangPoint_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void HangPoint_EditorDraw(void);
void HangPoint_EditorLoad(void);
#endif
void HangPoint_Serialize(void);

// Extra Entity Functions
void HangPoint_HandlePlayerMovement(EntityHangPoint *self, EntityPlayer *player, int32 playerID);

#endif //! OBJ_HANGPOINT_H
