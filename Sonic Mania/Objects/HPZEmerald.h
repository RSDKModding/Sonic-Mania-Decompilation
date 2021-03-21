#ifndef OBJ_HPZEMERALD_H
#define OBJ_HPZEMERALD_H

#include "../SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    ushort spriteIndex;
} ObjectHPZEmerald;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int field_58;
    int type;
    int field_60;
    Vector2 startPos;
    Hitbox *hitbox;
    AnimationData data2;
    AnimationData data;
} EntityHPZEmerald;

// Object Struct
extern ObjectHPZEmerald *HPZEmerald;

// Standard Entity Events
void HPZEmerald_Update();
void HPZEmerald_LateUpdate();
void HPZEmerald_StaticUpdate();
void HPZEmerald_Draw();
void HPZEmerald_Create(void* data);
void HPZEmerald_StageLoad();
void HPZEmerald_EditorDraw();
void HPZEmerald_EditorLoad();
void HPZEmerald_Serialize();

// Extra Entity Functions


#endif //!OBJ_HPZEMERALD_H
