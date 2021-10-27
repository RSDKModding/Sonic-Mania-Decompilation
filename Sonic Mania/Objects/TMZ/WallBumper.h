#ifndef OBJ_WALLBUMPER_H
#define OBJ_WALLBUMPER_H

#include "SonicMania.h"

// Object Class
typedef struct {
    RSDK_OBJECT
    uint16 aniFrames;
    uint16 sfxBouncer;
} ObjectWallBumper;

// Entity Class
typedef struct {
    RSDK_ENTITY
    int32 type;
    int32 size;
    bool32 reverse;
    bool32 destructible;
    Animator animator;
    Hitbox hitbox;
} EntityWallBumper;

// Object Struct
extern ObjectWallBumper *WallBumper;

// Standard Entity Events
void WallBumper_Update(void);
void WallBumper_LateUpdate(void);
void WallBumper_StaticUpdate(void);
void WallBumper_Draw(void);
void WallBumper_Create(void* data);
void WallBumper_StageLoad(void);
#if RETRO_INCLUDE_EDITOR
void WallBumper_EditorDraw(void);
void WallBumper_EditorLoad(void);
#endif
void WallBumper_Serialize(void);

// Extra Entity Functions
void WallBumper_DebugSpawn(void);
void WallBumper_DebugDraw(void);

void WallBumper_HandleInteractions(void);

#endif //!OBJ_WALLBUMPER_H
