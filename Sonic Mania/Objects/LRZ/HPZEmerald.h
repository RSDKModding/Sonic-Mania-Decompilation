#ifndef OBJ_HPZEMERALD_H
#define OBJ_HPZEMERALD_H

#include "SonicMania.h"

typedef enum {
    HPZEMERALD_MASTER,
    HPZEMERALD_EMERALD_LOW,
    HPZEMERALD_EMERALD_HIGH,
}HPZEmeraldTypes;

// Object Class
struct ObjectHPZEmerald {
    RSDK_OBJECT
    uint16 aniFrames;
};

// Entity Class
struct EntityHPZEmerald {
    RSDK_ENTITY
    int32 field_58;
    int32 type;
    bool32 solid;
    Vector2 startPos;
    Hitbox *hitbox;
    Animator animator2;
    Animator animator;
};

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
