#ifndef OBJ_PLAYERPROBE_H
#define OBJ_PLAYERPROBE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort spriteIndex;
} ObjectPlayerProbe;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int size;
    byte activePlayers;
    int negAngle;
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
void PlayerProbe_EditorDraw(void);
void PlayerProbe_EditorLoad(void);
void PlayerProbe_Serialize(void);

// Extra Entity Functions
void PlayerProbe_Print(EntityPlayer *player);
void PlayerProbe_DrawEditor(uint colour, int x1, int y1, int x2, int y2);

#endif //!OBJ_PLAYERPROBE_H
