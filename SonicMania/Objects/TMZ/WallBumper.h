#ifndef OBJ_WALLBUMPER_H
#define OBJ_WALLBUMPER_H

#include "Game.h"

typedef enum {
    WALLBUMPER_H,
    WALLBUMPER_V,
} WallBumperTypes;

// Object Class
struct ObjectWallBumper {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxBouncer;
};

// Entity Class
struct EntityWallBumper {
    RSDK_ENTITY
    int32 type;
    int32 size;
    bool32 reverse;
    bool32 destructible;
    Animator animator;
    Hitbox hitbox;
};

// Object Struct
extern ObjectWallBumper *WallBumper;

// Standard Entity Events
void WallBumper_Update(void);
void WallBumper_LateUpdate(void);
void WallBumper_StaticUpdate(void);
void WallBumper_Draw(void);
void WallBumper_Create(void *data);
void WallBumper_StageLoad(void);
#if GAME_INCLUDE_EDITOR
void WallBumper_EditorDraw(void);
void WallBumper_EditorLoad(void);
#endif
void WallBumper_Serialize(void);

// Extra Entity Functions
void WallBumper_DebugSpawn(void);
void WallBumper_DebugDraw(void);

void WallBumper_HandleInteractions(void);

#endif //! OBJ_WALLBUMPER_H
