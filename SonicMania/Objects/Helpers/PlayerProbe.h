#ifndef OBJ_PLAYERPROBE_H
#define OBJ_PLAYERPROBE_H

#include "Game.h"

// Object Class
struct ObjectPlayerProbe {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityPlayerProbe {
    RSDK_ENTITY
    int32 size;
    uint8 activePlayers;
    int32 negAngle;
    Animator animator;
};

// Object Struct
extern ObjectPlayerProbe *PlayerProbe;

// Standard Entity Events
void PlayerProbe_Update(void);
void PlayerProbe_LateUpdate(void);
void PlayerProbe_StaticUpdate(void);
void PlayerProbe_Draw(void);
void PlayerProbe_Create(void *data);
void PlayerProbe_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void PlayerProbe_EditorDraw(void);
void PlayerProbe_EditorLoad(void);
#endif
void PlayerProbe_Serialize(void);

// Extra Entity Functions
void PlayerProbe_Print(EntityPlayer *player);
void PlayerProbe_DrawSprites(void);
void PlayerProbe_DrawArrow(int32 x1, int32 y1, int32 x2, int32 y2, uint32 color);

#endif //! OBJ_PLAYERPROBE_H
