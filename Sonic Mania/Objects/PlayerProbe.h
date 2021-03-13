#ifndef OBJ_PLAYERPROBE_H
#define OBJ_PLAYERPROBE_H

#include "../SonicMania.h"

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
    EntityAnimationData data;
} EntityPlayerProbe;

// Object Struct
extern ObjectPlayerProbe *PlayerProbe;

// Standard Entity Events
void PlayerProbe_Update();
void PlayerProbe_LateUpdate();
void PlayerProbe_StaticUpdate();
void PlayerProbe_Draw();
void PlayerProbe_Create(void* data);
void PlayerProbe_StageLoad();
void PlayerProbe_EditorDraw();
void PlayerProbe_EditorLoad();
void PlayerProbe_Serialize();

// Extra Entity Functions
void PlayerProbe_Print(EntityPlayer *player);
void PlayerProbe_DrawEditor(int x1, int y1, int x2, int y2);
void PlayerProbe_DrawDebug2(uint colour, byte dir, int x, int y, Hitbox *hitbox);
void PlayerProbe_DrawDebug3(int x1, int y1, int x2, int y2);
void PlayerProbe_DrawDebug4(uint colour, int a2, int alpha, int x1, int y1, int x2, int y2);
void PlayerProbe_DrawDebug5(int x, int y);

#endif //!OBJ_PLAYERPROBE_H
