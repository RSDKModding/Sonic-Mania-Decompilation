#ifndef OBJ_PLAYERPROBE_H
#define OBJ_PLAYERPROBE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
} ObjectPlayerProbe;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int32 size;
    uint8 activePlayers;
    int32 negAngle;
    Animator animator;
} EntityPlayerProbe;

// Object Struct
extern ObjectPlayerProbe *PlayerProbe;

// Standard Entity Events
void PlayerProbe_Update(void);
void PlayerProbe_LateUpdate(void);
void PlayerProbe_StaticUpdate(void);
void PlayerProbe_Draw(void);
void PlayerProbe_Create(void* data);
void PlayerProbe_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void PlayerProbe_EditorDraw(void);
void PlayerProbe_EditorLoad(void);
#endif
void PlayerProbe_Serialize(void);

// Extra Entity Functions
void PlayerProbe_Print(EntityPlayer *player);
void PlayerProbe_DrawSprites(void);
void PlayerProbe_DrawEditor(uint32 colour, int32 x1, int32 y1, int32 x2, int32 y2);

#endif //!OBJ_PLAYERPROBE_H
