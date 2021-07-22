#ifndef OBJ_ICE_H
#define OBJ_ICE_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort aniFrames;
    Hitbox hitbox1;
    Hitbox hitbox2;
    int playerTimers[PLAYER_MAX];
    ushort sfx_Freeze;
    ushort sfx_LedgeBreak;
    ushort sfx_WindowShatter;
    ushort sfx_Struggle;
} ObjectIce;

// Entity Class
typedef struct {
	RSDK_ENTITY
} EntityIce;

// Object Struct
extern ObjectIce *Ice;

// Standard Entity Events
void Ice_Update(void);
void Ice_LateUpdate(void);
void Ice_StaticUpdate(void);
void Ice_Draw(void);
void Ice_Create(void* data);
void Ice_StageLoad(void);
void Ice_EditorDraw(void);
void Ice_EditorLoad(void);
void Ice_Serialize(void);

// Extra Entity Functions
void Ice_State_FrozenPlayer(void);

void Ice_ZoneCB(void);
void Ice_Unknown8(Entity *player);
void Ice_TimeOverCB(void);

#endif //!OBJ_ICE_H
