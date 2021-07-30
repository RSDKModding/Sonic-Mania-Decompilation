#ifndef OBJ_HANGPOINT_H
#define OBJ_HANGPOINT_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    Animator animator;
    ushort aniFrames;
    Hitbox hitbox;
} ObjectHangPoint;

// Entity Class
typedef struct {
    RSDK_ENTITY
    byte activePlayers;
    int field_5C[4];
    int playerTimer[4];
    int field_7C;
    int field_80;
    int length;
    byte speed;
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
void HangPoint_EditorDraw(void);
void HangPoint_EditorLoad(void);
void HangPoint_Serialize(void);

// Extra Entity Functions
void HangPoint_Unknown1(EntityHangPoint *entity, void *p, int playerID);

#endif //!OBJ_HANGPOINT_H
