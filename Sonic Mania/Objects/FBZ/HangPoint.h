#ifndef OBJ_HANGPOINT_H
#define OBJ_HANGPOINT_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Animator animator;
    uint16 aniFrames;
    Hitbox hitbox;
} ObjectHangPoint;

// Entity Class
typedef struct {
    RSDK_ENTITY
    uint8 activePlayers;
    int32 moveDistance[4];
    int32 playerTimer[4];
    int32 pullPos;
    int32 maxVel;
    int32 length;
    uint8 speed;
} EntityHangPoint;

// Object Struct
extern ObjectHangPoint *HangPoint;

// Standard Entity Events
void HangPoint_Update(void);
void HangPoint_LateUpdate(void);
void HangPoint_StaticUpdate(void);
void HangPoint_Draw(void);
void HangPoint_Create(void* data);
void HangPoint_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void HangPoint_EditorDraw(void);
void HangPoint_EditorLoad(void);
#endif
void HangPoint_Serialize(void);

// Extra Entity Functions
void HangPoint_HandlePlayerMovement(EntityHangPoint *self, void *p, int32 playerID);

#endif //!OBJ_HANGPOINT_H
