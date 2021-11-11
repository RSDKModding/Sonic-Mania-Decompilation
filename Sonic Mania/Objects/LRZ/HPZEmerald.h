#ifndef OBJ_HPZEMERALD_H
#define OBJ_HPZEMERALD_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 spriteIndex;
} ObjectHPZEmerald;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int32 field_58;
    int32 type;
    bool32 flag;
    Vector2 startPos;
    Hitbox *hitbox;
    Animator animator2;
    Animator animator;
} EntityHPZEmerald;

// Object Struct
extern ObjectHPZEmerald *HPZEmerald;

// Standard Entity Events
void HPZEmerald_Update(void);
void HPZEmerald_LateUpdate(void);
void HPZEmerald_StaticUpdate(void);
void HPZEmerald_Draw(void);
void HPZEmerald_Create(void* data);
void HPZEmerald_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void HPZEmerald_EditorDraw(void);
void HPZEmerald_EditorLoad(void);
#endif
void HPZEmerald_Serialize(void);

// Extra Entity Functions


#endif //!OBJ_HPZEMERALD_H
